#include "th17.h"
#include "clientplayer.h"
#include "engine.h"
#include "general.h"
#include "skill.h"
#include "testCard.h"


/*
class ZaoxingVS : public ViewAsSkill
{
public:
    ZaoxingVS()
        : ViewAsSkill("zaoxing")
    {
    }

    bool viewFilter(const QList<const Card *> &selected, const Card *to_select) const override
    {
        if (Self->hasEquip(to_select))
            return false;

        // ??
        // if (to_select->getSuit() == Card::NoSuit)
        //     return false;

        if (Sanguosha->getCurrentCardUsePattern() == "@@zaoxing-card1") {
            if (!selected.isEmpty()) {
                if (to_select->getSuit() != selected.first()->getSuit())
                    return false;
            }
            return !Self->getShownHandcards().contains(to_select->getEffectiveId());
        }

        if (Self->getShownHandcards().contains(to_select->getEffectiveId())) {
            if ((to_select->isRed() && Self->property("zaoxing2").toString() == "red") || (to_select->isBlack() && Self->property("zaoxing2").toString() == "black")
                || (to_select->getColor() == Card::Colorless && Self->property("zaoxing2").toString() == "colorless"))
                return true;
        }

        return false;
    }

    const Card *viewAs(const QList<const Card *> &cards) const override
    {
        if (!cards.isEmpty()) {
            DummyCard *d = new DummyCard;
            d->addSubcards(cards);
            return d;
        }
        return nullptr;
    }

    bool isEnabledAtPlay(const Player *) const override
    {
        return false;
    }

    bool isEnabledAtResponse(const Player *, const QString &pattern) const override
    {
        return pattern.startsWith("@@zaoxing-card");
    }
};

class Zaoxing : public TriggerSkill
{
public:
    Zaoxing()
        : TriggerSkill("zaoxing")
    {
        events << EventPhaseStart << TargetConfirmed;
        view_as_skill = new ZaoxingVS;
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent triggerEvent, const Room *room, const QVariant &data) const override
    {
        if (triggerEvent == EventPhaseStart) {
            ServerPlayer *p = data.value<ServerPlayer *>();
            if (p->isAlive() && p->hasSkill(this) && p->getPhase() == Player::Play)
                return {SkillInvokeDetail(this, p, p)};
        } else {
            CardUseStruct use = data.value<CardUseStruct>();
            if (use.to.length() == 1) {
                QList<SkillInvokeDetail> r;
                foreach (ServerPlayer *p, room->getOtherPlayers(use.to.first())) {
                    if (p->isAlive() && p->hasSkill(this)) {
                        foreach (int id, p->getShownHandcards()) {
                            if (Sanguosha->getCard(id)->getColor() == use.card->getColor()) {
                                r << SkillInvokeDetail(this, p, p);
                                break;
                            }
                        }
                    }
                }

                return r;
            }
        }

        return {};
    }

    bool cost(TriggerEvent triggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &data) const override
    {
        const Card *c = nullptr;

        if (triggerEvent == EventPhaseStart) {
            c = room->askForCard(invoke->invoker, "@@zaoxing-card1", "@zaoxing-show", data, Card::MethodNone, nullptr, false, "zaoxing", false, 1);
            if (c != nullptr) {
                LogMessage log;
                log.type = "#InvokeSkill";
                log.from = invoke->invoker;
                log.arg = "zaoxing";
                room->sendLog(log);
            }
        } else {
            CardUseStruct use = data.value<CardUseStruct>();
            room->setPlayerProperty(invoke->invoker, "zaoxing2", QString(use.card->isBlack() ? "black" : (use.card->isRed() ? "red" : "colorless")));
            c = room->askForCard(invoke->invoker, "@@zaoxing-card2", "@zaoxing-recast", data, Card::MethodNone, nullptr, false, "zaoxing", false, 2);
        }

        if (c != nullptr) {
            // card operation should be procedured in effect
            QList<int> subcards = c->getSubcards();
            invoke->tag["zaoxing"] = IntList2VariantList(subcards);
            return true;
        }

        return false;
    }

    bool effect(TriggerEvent triggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &data) const override
    {
        QList<int> cards = VariantList2IntList(invoke->tag.value("zaoxing").toList());

        if (triggerEvent == EventPhaseStart)
            invoke->invoker->addToShownHandCards(cards);
        else {
            DummyCard d(cards);

            LogMessage log;
            log.type = "#Card_Recast";
            log.from = invoke->invoker;
            log.card_str = IntList2StringList(cards).join("+");
            room->sendLog(log);

            CardMoveReason reason(CardMoveReason::S_REASON_RECAST, invoke->invoker->objectName());
            room->moveCardTo(&d, invoke->invoker, nullptr, Player::DiscardPile, reason);
            invoke->invoker->broadcastSkillInvoke("@recast");

            invoke->invoker->drawCards(cards.length());

            if (cards.length() > 1) {
                CardUseStruct use = data.value<CardUseStruct>();
                use.to.first()->drawCards(1, "zaoxing");
            }
        }

        return false;
    }
};
*/


class Shanxing : public TriggerSkill
{
public:
    Shanxing()
        : TriggerSkill("shanxing")
    {
        events << CardsMoveOneTime;
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent, const Room *room, const QVariant &data) const override
    {
        CardsMoveOneTimeStruct move = data.value<CardsMoveOneTimeStruct>();
        ServerPlayer *ko = qobject_cast<ServerPlayer *>(move.from);
        if (ko != nullptr && ko->isAlive() && move.from_places.contains(Player::PlaceEquip)) {
            QList<SkillInvokeDetail> d;

            foreach(ServerPlayer *p, room->findPlayersBySkillName(objectName()))
                d << SkillInvokeDetail(this, p, p, nullptr, false, ko);
            return d;
        }

        return QList<SkillInvokeDetail>();
    }



    bool effect(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const override
    {
        RecoverStruct recover;
        recover.who = invoke->invoker;
        room->recover(invoke->targets.first(), recover);

        invoke->targets.first()->drawCards(1, objectName());
        if (invoke->targets.first() != invoke->invoker) {
            const Card *c = room->askForExchange(invoke->targets.first(), objectName(), 1, 1, false, "@shanxing-exchange:" + invoke->invoker->objectName(), true);
            if (c != nullptr) {
                CardMoveReason reason(CardMoveReason::S_REASON_GIVE, invoke->targets.first()->objectName(), objectName(), QString());
                room->obtainCard(invoke->invoker, c, reason, false);
            }
        }
        return false;
    }
};


class LingshouOtherVS : public ViewAsSkill
{
public:
    LingshouOtherVS()
        : ViewAsSkill("LingshouOtherVS")
    {
        response_or_use = true;
        response_pattern = "@@LingshouOtherVS";
    }

    bool viewFilter(const QList<const Card *> &, const Card *to_select) const override
    {
        QList<int> selectedIds = StringList2IntList(Self->property("lingshouSelected").toString().split("+"));
        return selectedIds.contains(to_select->getId());
    }

    const Card *viewAs(const QList<const Card *> &cards) const override
    {
        Slash *s = new Slash(Card::SuitToBeDecided, -1);
        s->addSubcards(cards);
        s->setSkillName("_lingshou");

        if (s->getSubcards().toSet() == StringList2IntList(Self->property("lingshouSelected").toString().split("+")).toSet())
            return s;

        delete s;
        return nullptr;
    }
};

class Lingshou : public TriggerSkill
{
public:
    Lingshou()
        : TriggerSkill("lingshou")
    {
        events << EventPhaseStart;
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent, const Room *room, const QVariant &data) const override
    {
        ServerPlayer *p = data.value<ServerPlayer *>();
        if (p->isAlive() && p->hasSkill(this) && p->getPhase() == Player::Finish) {
            foreach (ServerPlayer *ps, room->getOtherPlayers(p)) {
                if (!ps->isKongcheng())
                    return {SkillInvokeDetail(this, p, p)};
            }
        }

        return {};
    }

    bool cost(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const override
    {
        QList<ServerPlayer *> targets;
        foreach (ServerPlayer *ps, room->getOtherPlayers(invoke->invoker)) {
            if (!ps->isKongcheng())
                targets << ps;
        }

        ServerPlayer *target = room->askForPlayerChosen(invoke->invoker, targets, "lingshou", "@lingshou-kill", true, true);
        if (target != nullptr) {
            QList<int> ids = target->handCards();

            room->fillAG(ids, invoke->invoker);
            int id1 = room->askForAG(invoke->invoker, ids, false, "lingshou");
            if (id1 == -1)
                id1 = ids.first();
            room->clearAG(invoke->invoker); // re-fill AG since the disabled cards changed

            QList<int> disable_ids;
            QList<int> ids2;
            foreach (int id, ids) {
                if ((Sanguosha->getCard(id)->getSuit() == Sanguosha->getCard(id1)->getSuit()) && (id != id1))
                    ids2 << id;
                else
                    disable_ids << id;
            }

            int id2 = -1;
            if (!ids2.isEmpty()) {
                room->fillAG(ids, invoke->invoker, disable_ids);
                id2 = room->askForAG(invoke->invoker, ids2, true, "lingshou");
                room->clearAG(invoke->invoker);
            }

            room->showCard(target, id1);
            QList<int> selectedIds {id1};
            if (id2 != -1) {
                selectedIds << id2;
                room->showCard(target, id2);
            }

            invoke->tag["lingshou"] = IntList2VariantList(selectedIds);
            invoke->targets << target;
            return true;
        }

        return false;
    }

    bool effect(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const override
    {
        QList<int> selectedIds = VariantList2IntList(invoke->tag.value("lingshou").toList());
        room->setPlayerProperty(invoke->targets.first(), "lingshouSelected", IntList2StringList(selectedIds).join("+"));
        if (room->askForUseCard(invoke->targets.first(), "@@LingshouOtherVS", "@lingshou-slash", -1, Card::MethodUse, true, "_lingshou") == nullptr) {
            DummyCard d(selectedIds);

            LogMessage log;
            log.type = "#Card_Recast";
            log.from = invoke->targets.first();
            log.card_str = IntList2StringList(selectedIds).join("+");
            room->sendLog(log);

            CardMoveReason reason(CardMoveReason::S_REASON_RECAST, invoke->targets.first()->objectName());
            room->moveCardTo(&d, invoke->targets.first(), nullptr, Player::DiscardPile, reason);
            invoke->targets.first()->broadcastSkillInvoke("@recast");

            invoke->targets.first()->drawCards(selectedIds.length());
        }

        return false;
    }
};

/*
ZhuyingCard::ZhuyingCard()
{
    m_skillName = "zhuying";
    will_throw = false;
    handling_method = MethodNone;
}

void ZhuyingCard::onUse(Room *room, const CardUseStruct &card_use) const
{
    CardUseStruct use = card_use;
    QList<ServerPlayer *> lords;

    foreach (ServerPlayer *p, room->getOtherPlayers(use.from)) {
        if (p->hasLordSkill("zhuying") && !p->hasFlag("zhuying_selected"))
            lords << p;
    }

    ServerPlayer *lord = lords.first();
    if (lords.length() > 1)
        lord = room->askForPlayerChosen(use.from, lords, "zhuying-multilord", "@zhuying-multilord");
    if (lord == nullptr)
        lord = lords.first();

    lord->tag["zhuying_current"] = QVariant::fromValue<ServerPlayer *>(use.from);
    use.from = lord;

    CardsMoveStruct move(getEffectiveId(), lord, Player::PlaceHand, CardMoveReason(CardMoveReason::S_REASON_GIVE, use.from->objectName()));
    room->moveCardsAtomic(move, false);

    SkillCard::onUse(room, use);
}

void ZhuyingCard::onEffect(const CardEffectStruct &effect) const
{
    Room *r = effect.from->getRoom();
    r->setPlayerFlag(effect.from, "zhuying_selected");

    if (r->askForSkillInvoke(effect.from, "zhuying-effect", "effect:" + effect.to->objectName())) {
        LogMessage l;
        l.type = "#zhuying-bribe";
        l.from = effect.from;
        l.to << effect.to;
        r->sendLog(l);

        QStringList e = effect.to->property("zhuying_effect").toString().split("+");
        e << effect.from->objectName();
        r->setPlayerProperty(effect.to, "zhuying_effect", e.join("+"));
    }
}

class ZhuyingAttach : public OneCardViewAsSkill
{
public:
    ZhuyingAttach()
        : OneCardViewAsSkill("zhuying_attach")
    {
        filter_pattern = ".|.|.|hand";
        attached_lord_skill = true;
    }

    bool isEnabledAtPlay(const Player *player) const
    {
        if (player->getKingdom() != "gxs")
            return false;

        foreach (const Player *p, player->getAliveSiblings()) {
            if (p->hasLordSkill("zhuying") && !p->hasFlag("zhuying_selected"))
                return true;
        }

        return false;
    }

    const Card *viewAs(const Card *originalCard) const override
    {
        ZhuyingCard *c = new ZhuyingCard;
        c->addSubcard(originalCard);
        return c;
    }
};

class Zhuying : public TriggerSkill
{
public:
    Zhuying()
        : TriggerSkill("zhuying$")
    {
        events << GameStart << EventAcquireSkill << EventLoseSkill << Death << Debut << Revive << GeneralShown << EventPhaseChanging << EventPhaseStart;
        show_type = "static";
    }

    void record(TriggerEvent e, Room *room, QVariant &d) const override
    {
        if (e == EventPhaseChanging) {
            PhaseChangeStruct change = d.value<PhaseChangeStruct>();
            if (change.from == Player::Play) {
                foreach (ServerPlayer *p, room->getAllPlayers()) {
                    if (p->hasFlag("zhuying_selected"))
                        room->setPlayerFlag(p, "-zhuying_selected");
                }
            }
        } else if (e == EventPhaseStart) {
            ServerPlayer *p = d.value<ServerPlayer *>();
            if (p->getPhase() == Player::NotActive) {
                foreach (ServerPlayer *ps, room->getAllPlayers()) {
                    if (!ps->property("zhuying_effect").toString().isEmpty())
                        room->setPlayerProperty(ps, "zhuying_effect", QString());
                }
            }
        } else {
            static QString attachName = "zhuying_attach";
            QList<ServerPlayer *> sklts;
            foreach (ServerPlayer *p, room->getAllPlayers()) {
                if (p->hasLordSkill(this, true))
                    sklts << p;
            }

            if (sklts.length() > 1) {
                foreach (ServerPlayer *p, room->getAllPlayers()) {
                    if (!p->hasSkill(attachName, true))
                        room->attachSkillToPlayer(p, attachName);
                }
            } else if (sklts.length() == 1) {
                foreach (ServerPlayer *p, room->getAllPlayers()) {
                    if (p->hasLordSkill(this, true) && p->hasSkill(attachName, true))
                        room->detachSkillFromPlayer(p, attachName, true);
                    else if (!p->hasLordSkill(this, true) && !p->hasSkill(attachName, true))
                        room->attachSkillToPlayer(p, attachName);
                }
            } else { // the case that sklts is empty
                foreach (ServerPlayer *p, room->getAllPlayers()) {
                    if (p->hasSkill(attachName, true))
                        room->detachSkillFromPlayer(p, attachName, true);
                }
            }
        }
    }
};

class ZhuyingD : public DistanceSkill
{
public:
    ZhuyingD()
        : DistanceSkill("#zhuying-distance")
    {
    }

    int getCorrect(const Player *from, const Player *to) const override
    {
        if (from == to)
            return 0;

        if (!to->property("zhuying_effect").toString().isEmpty()) {
            QStringList slist = to->property("zhuying_effect").toString().split("+");
            return -(slist.length() - slist.count(from->objectName()));
        }

        return 0;
    }
};
*/

class Qijue : public TriggerSkill
{
public:
    Qijue()
        : TriggerSkill("qijue$")
    {
        events << Death << BuryVictim;
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent e, const Room *room, const QVariant &data) const override
    {
        DeathStruct death = data.value<DeathStruct>();
        QList<SkillInvokeDetail> details;
        if (e == Death) {
            if (death.who->getKingdom() != "gxs")
                return details;

            foreach(ServerPlayer *p, room->getOtherPlayers(death.who)) {
                if (p->hasLordSkill(objectName()) && p->isWounded())
                    details << SkillInvokeDetail(this, p, death.who);
            }
        }
        else if (e == BuryVictim) {
            ServerPlayer *killer = death.damage->from;
            if (killer == nullptr || killer->getKingdom() != "gxs")
                return details;

            foreach(ServerPlayer *p, room->getOtherPlayers(killer)) {
                if (p->hasLordSkill(objectName()) && p->isWounded())
                    details << SkillInvokeDetail(this, p, killer);
            }
        }
        return details;
    }

    bool cost(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const override
    {
        bool result = invoke->invoker->askForSkillInvoke(objectName(), QVariant::fromValue(invoke->owner));
        if (result) {
            room->broadcastSkillInvoke(objectName());

            room->doAnimate(QSanProtocol::S_ANIMATE_INDICATE, invoke->invoker->objectName(), invoke->owner->objectName());
            room->notifySkillInvoked(invoke->owner, objectName());
            LogMessage log;
            log.type = "#InvokeOthersSkill";
            log.from = invoke->invoker;
            log.to << invoke->owner;
            log.arg = objectName();
            room->sendLog(log);
        }
        return result;
    }

    bool effect(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const override
    {
        RecoverStruct recover;
        recover.who = invoke->invoker;
        room->recover(invoke->owner, recover);

        return false;
    }
};



class Shanlei : public TriggerSkill
{
public:
    Shanlei()
        : TriggerSkill("shanlei")
    {
        events << EventPhaseStart;
        frequency = Compulsory;
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent, const Room *room, const QVariant &data) const override
    {
        QList<SkillInvokeDetail> r;

        ServerPlayer *player = data.value<ServerPlayer *>();
        if (player->getPhase() == Player::Start) {
            if (player->isAlive() && player->hasSkill(this) && player->getHandcardNum() > player->getMaxCards())
                r << SkillInvokeDetail(this, player, player, nullptr, true);
        } else if (player->getPhase() == Player::Finish) {
            if (player->isAlive() && player->hasSkill(this)) {
                bool flag = false;
                foreach (ServerPlayer *p, room->getOtherPlayers(player)) {
                    if (p->getHandcardNum() >= player->getHandcardNum()) {
                        flag = true;
                        break;
                    }
                }
                if (flag)
                    r << SkillInvokeDetail(this, player, player, nullptr, true);
            }
        }

        return r;
    }

    bool cost(TriggerEvent triggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &data) const override
    {
        if (TriggerSkill::cost(triggerEvent, room, invoke, data)) {
            if (invoke->invoker->hasShownSkill(this)) {
                LogMessage l;
                l.type = "#TriggerSkill";
                l.from = invoke->invoker;
                l.arg = objectName();
                room->sendLog(l);
            }
            return true;
        }

        return false;
    }

    bool effect(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const override
    {
        if (invoke->invoker->getPhase() == Player::Start) {
            // it is certain that p->getHandcardNum() > p->getMaxCards() equals true
            int n = invoke->invoker->getHandcardNum() - invoke->invoker->getMaxCards();
            room->askForDiscard(invoke->invoker, objectName(), n, n, false, false, "@shanlei-discard");
        } else {
            int cardnumMost = 0;
            foreach (ServerPlayer *p, room->getOtherPlayers(invoke->invoker)) {
                if (p->getHandcardNum() >= cardnumMost)
                    cardnumMost = p->getHandcardNum();
            }
            int n = cardnumMost - invoke->invoker->getHandcardNum() + 1;
            room->drawCards(invoke->invoker, n, "shanlei");
        }

        return false;
    }
};

class BengluoVS : public ViewAsSkill
{
public:
    BengluoVS()
        : ViewAsSkill("bengluo")
    {
    }

    bool isResponseOrUse() const override
    {
        return Sanguosha->getCurrentCardUsePattern() == "@@bengluo-card1";
    }

    bool viewFilter(const QList<const Card *> &selected, const Card *to_select) const override
    {
        if (Sanguosha->getCurrentCardUsePattern() == "@@bengluo-card1") {
            if (!selected.isEmpty())
                return false;
        } else {
            bool ok = false;
            if ((selected.length() >= Self->property("bengluoDiscardnum").toString().toInt(&ok)) || !ok)
                return false;

            if (Self->isJilei(to_select))
                return false;
        }
        return !to_select->isEquipped();
    }

    const Card *viewAs(const QList<const Card *> &cards) const override
    {
        if (Sanguosha->getCurrentCardUsePattern() == "@@bengluo-card1") {
            if (!cards.isEmpty()) {
                Slash *slash = new Slash(Card::SuitToBeDecided, -1);
                slash->addSubcards(cards);
                slash->setShowSkill("bengluo");
                slash->setSkillName("bengluo");
                return slash;
            }
        } else {
            bool ok = false;
            if ((cards.length() == Self->property("bengluoDiscardnum").toString().toInt(&ok)) && ok) {
                DummyCard *card = new DummyCard;
                card->addSubcards(cards);
                return card;
            }
        }

        return nullptr;
    }

    bool isEnabledAtPlay(const Player *) const override
    {
        return false;
    }

    bool isEnabledAtResponse(const Player *, const QString &pattern) const override
    {
        return pattern.startsWith("@@bengluo-card");
    }
};

class Bengluo : public TriggerSkill
{
public:
    Bengluo()
        : TriggerSkill("bengluo")
    {
        events << CardsMoveOneTime << TurnStart << EventPhaseEnd << EventPhaseStart << DamageCaused;
        view_as_skill = new BengluoVS;
        global = true;
    }

    void record(TriggerEvent triggerEvent, Room *room, QVariant &data) const override
    {
        if (triggerEvent == CardsMoveOneTime) {
            CardsMoveOneTimeStruct move = data.value<CardsMoveOneTimeStruct>();

            if ((move.from != nullptr) && (move.from_places.contains(Player::PlaceHand) || move.from_places.contains(Player::PlaceEquip))
                && ((move.reason.m_reason & CardMoveReason::S_MASK_BASIC_REASON) != CardMoveReason::S_REASON_USE)
                && ((move.to != move.from) || ((move.to_place != Player::PlaceHand) && (move.to_place != Player::PlaceEquip)))) {
                ServerPlayer *current = room->getCurrent();
                if (current != nullptr && current->isInMainPhase() && current->isAlive()) {
                    if (move.from->getHandcardNum() > move.from->getMaxCards())
                        move.from->setFlags("bengluo");
                }
            }
        } else if (triggerEvent == TurnStart) {
            foreach (ServerPlayer *p, room->getAllPlayers())
                p->setFlags("-bengluo");
        }
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent triggerEvent, const Room *room, const QVariant &data) const override
    {
        if (triggerEvent == EventPhaseEnd) {
            ServerPlayer *change = data.value<ServerPlayer *>();
            if (change->isInMainPhase()) {
                QList<SkillInvokeDetail> r;
                foreach (ServerPlayer *p, room->getAllPlayers()) {
                    if (p->hasSkill(this) && p->hasFlag("bengluo"))
                        r << SkillInvokeDetail(this, p, p);
                }
                return r;
            }
        } else if (triggerEvent == DamageCaused) {
            DamageStruct damage = data.value<DamageStruct>();
            if (damage.card != nullptr && damage.card->isKindOf("Slash") && damage.card->getSkillName() == "bengluo" && damage.by_user && !damage.chain && !damage.transfer
                && damage.from != nullptr && damage.from->isAlive() && damage.from->hasSkill(this) && damage.to->getHandcardNum() < damage.from->getHandcardNum()
                && damage.from->getHandcardNum() != damage.from->getMaxCards())
                return {SkillInvokeDetail(this, damage.from, damage.from)};
        }

        return QList<SkillInvokeDetail>();
    }

    bool cost(TriggerEvent triggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &data) const override
    {
        if (triggerEvent == EventPhaseEnd)
            return room->askForUseCard(invoke->invoker, "@@bengluo-card1", "@bengluo-kill", 1) != nullptr;
        else {
            int n = invoke->invoker->getHandcardNum() - invoke->invoker->getMaxCards();
            if (n > 0) {
                room->setPlayerProperty(invoke->invoker, "bengluoDiscardnum", QString::number(n));
                return room->askForCard(invoke->invoker, "@@bengluo-card2", "@bengluo-discard:::" + QString::number(n), data, "bengluo", 2) != nullptr;
            } else {
                if (room->askForSkillInvoke(invoke->invoker, this, data, "@bengluo-draw:::" + QString::number(-n))) {
                    room->drawCards(invoke->invoker, -n, "bengluo");
                    return true;
                }
            }
        }

        return false;
    }

    bool effect(TriggerEvent triggerEvent, Room *r, QSharedPointer<SkillInvokeDetail>, QVariant &data) const override
    {
        if (triggerEvent == DamageCaused) {
            DamageStruct damage = data.value<DamageStruct>();

            LogMessage l;
            l.type = "#mofa_damage";
            l.from = damage.from;
            l.to << damage.to;
            l.arg = QString::number(damage.damage + 1);
            l.arg2 = QString::number(damage.damage);
            r->sendLog(l);

            damage.damage += 1;
            data = QVariant::fromValue<DamageStruct>(damage);
        }

        return false;
    }
};

LunniCard::LunniCard()
{
    target_fixed = true;
    will_throw = false;
    handling_method = Card::MethodNone;
}

void LunniCard::onUse(Room *room, const CardUseStruct &card_use) const
{
    CardUseStruct use = card_use;
    ServerPlayer *current = room->getCurrent();

    Q_ASSERT(current != nullptr);
    Q_ASSERT((current->isAlive() && current->getPhase() == Player::Play));

    use.to << current;
    SkillCard::onUse(room, use);
}

void LunniCard::onEffect(const CardEffectStruct &effect) const
{
    const EquipCard *c = qobject_cast<const EquipCard *>(Sanguosha->getCard(effect.card->getSubcards().first())->getRealCard());
    if (c == nullptr)
        return;

    EquipCard::Location location = c->location();

    int equipped_id = Card::S_UNKNOWN_CARD_ID;
    ServerPlayer *target = effect.to;
    Room *room = target->getRoom();
    if (target->getEquip(location) != nullptr)
        equipped_id = target->getEquip(location)->getEffectiveId();

    QList<CardsMoveStruct> exchangeMove;
    CardsMoveStruct move1(subcards.first(), target, Player::PlaceEquip, CardMoveReason(CardMoveReason::S_REASON_PUT, target->objectName()));
    exchangeMove.push_back(move1);
    if (equipped_id != Card::S_UNKNOWN_CARD_ID) {
        CardsMoveStruct move2(equipped_id, nullptr, Player::DiscardPile, CardMoveReason(CardMoveReason::S_REASON_CHANGE_EQUIP, target->objectName()));
        exchangeMove.push_back(move2);
    }
    room->moveCardsAtomic(exchangeMove, true);

    effect.to->setFlags("lunni");
}

class LunniVS : public OneCardViewAsSkill
{
public:
    LunniVS()
        : OneCardViewAsSkill("lunni")
    {
        filter_pattern = "EquipCard";
        response_pattern = "@@lunni";
    }

    const Card *viewAs(const Card *originalCard) const override
    {
        LunniCard *c = new LunniCard;
        c->addSubcard(originalCard);
        c->setShowSkill("lunni");
        return c;
    }
};

class Lunni : public TriggerSkill
{
public:
    Lunni()
        : TriggerSkill("lunni")
    {
        events << EventPhaseStart << EventPhaseEnd;
        view_as_skill = new LunniVS;
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent triggerEvent, const Room *room, const QVariant &data) const override
    {
        QList<SkillInvokeDetail> r;

        if (triggerEvent == EventPhaseStart) {
            ServerPlayer *p = data.value<ServerPlayer *>();
            if (p->getPhase() == Player::Play) {
                QList<ServerPlayer *> players = room->getOtherPlayers(p);
                foreach (ServerPlayer *bull, players) {
                    if (bull->isAlive() && bull->hasSkill(this) && !bull->getCards("e").isEmpty())
                        r << SkillInvokeDetail(this, bull, bull, p);
                }
            }
        } else if (triggerEvent == EventPhaseEnd) {
            ServerPlayer *change = data.value<ServerPlayer *>();
            if (change->hasFlag("lunni")) {
                if (change->getPhase() == Player::Play && !change->getCards("e").isEmpty())
                    r << SkillInvokeDetail(this, change, change, nullptr, true, nullptr, false);
            }
        }

        return r;
    }

    bool cost(TriggerEvent triggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &data) const override
    {
        if (triggerEvent == EventPhaseStart) {
            return room->askForUseCard(invoke->invoker, "@@lunni", "@lunni-discard:" + invoke->targets.first()->objectName(), -1, Card::MethodNone) != nullptr;
        } else {
            ServerPlayer *change = data.value<ServerPlayer *>();
            if (change->hasFlag("lunni") && invoke->invoker == change)
                return true;
        }

        return false;
    }

    bool effect(TriggerEvent triggerEvent, Room *room, QSharedPointer<SkillInvokeDetail>, QVariant &data) const override
    {
        if (triggerEvent == EventPhaseEnd) {
            ServerPlayer *change = data.value<ServerPlayer *>();
            if (change->hasFlag("lunni")) {
                LogMessage l;
                l.type = "#lunni-eff4";
                l.from = change;
                room->sendLog(l);

                DummyCard d;
                d.addSubcards(change->getCards("e"));
                change->obtainCard(&d);
            }
        }

        return false;
    }
};

class LiaoguVS : public OneCardViewAsSkill
{
public:
    LiaoguVS()
        : OneCardViewAsSkill("liaogu")
    {
        response_pattern = "@@liaogu";
        filter_pattern = ".|.|.|#liaoguTemp";
        expand_pile = "#liaoguTemp";
    }

    const Card *viewAs(const Card *originalCard) const override
    {
        LiaoguCard *c = new LiaoguCard;
        c->addSubcard(originalCard);
        return c;
    }
};

class Liaogu : public TriggerSkill
{
public:
    // notify liaogu card move
    static void liaoguTempMove(Room *room, ServerPlayer *to, QList<int> inCards)
    {
        QList<int> inCardsCopy = inCards;
        QList<int> outCards = VariantList2IntList(to->tag.value("liaogu_tempmove", QVariantList()).toList());
        QList<CardsMoveStruct> moves;

        // remove duplicate
        foreach (int c, inCardsCopy) {
            if (outCards.contains(c)) {
                inCards.removeAll(c);
                outCards.removeAll(c);
            }
        }

        if (!outCards.isEmpty()) {
            CardsMoveStruct move;
            move.card_ids = outCards;
            move.open = true;
            move.from = to;
            move.from_player_name = to->objectName();
            move.from_place = Player::PlaceSpecial;
            move.from_pile_name = "#liaoguTemp";
            move.to = nullptr;
            move.to_place = Player::DiscardPile;

            moves << move;
        }

        if (!inCards.isEmpty()) {
            CardsMoveStruct move;
            move.card_ids = inCards;
            move.open = true;
            move.from = nullptr;
            move.from_place = Player::DiscardPile;
            move.to = to;
            move.to_player_name = to->objectName();
            move.to_place = Player::PlaceSpecial;
            move.to_pile_name = "#liaoguTemp";

            moves << move;
        }

        if (!inCardsCopy.isEmpty())
            to->tag["liaogu_tempmove"] = IntList2VariantList(inCardsCopy);
        else
            to->tag.remove("liaogu_tempmove");

        if (moves.isEmpty())
            return;

        room->setPlayerFlag(to, "liaogu_InTempMoving");
        room->notifyMoveCards(true, moves, true, {to});
        room->notifyMoveCards(false, moves, true, {to});
        room->setPlayerFlag(to, "-liaogu_InTempMoving");
    }

    Liaogu()
        : TriggerSkill("liaogu")
    {
        view_as_skill = new LiaoguVS;
        events << CardsMoveOneTime << EventPhaseChanging;
        global = true;
    }

    void record(TriggerEvent triggerEvent, Room *room, QVariant &data) const override
    {
        if (triggerEvent == CardsMoveOneTime) {
            CardsMoveOneTimeStruct move = data.value<CardsMoveOneTimeStruct>();
            if (move.from != nullptr && (move.from_places.contains(Player::PlaceHand) || move.from_places.contains(Player::PlaceEquip))
                && ((move.to != move.from) || (move.to_place != Player::PlaceHand && move.to_place != Player::PlaceEquip)))
                if (room->getCurrent() != nullptr && room->getCurrent()->isAlive() && room->getCurrent()->getPhase() != Player::NotActive)
                    move.from->setFlags("liaogulost");
        } else if (triggerEvent == EventPhaseChanging) {
            foreach (ServerPlayer *p, room->getAllPlayers())
                p->setFlags("-liaogulost");
        }
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent triggerEvent, const Room *room, const QVariant &data) const override
    {
        if (triggerEvent == CardsMoveOneTime) {
            CardsMoveOneTimeStruct move = data.value<CardsMoveOneTimeStruct>();
            if (move.reason.m_reason == CardMoveReason::S_REASON_RULEDISCARD && move.from != nullptr && move.from->isCurrent() && move.from->getPhase() == Player::Discard) {
                bool inDiscardPile = false;
                foreach (int cardid, move.card_ids) {
                    if (room->getCardPlace(cardid) == Player::DiscardPile) {
                        inDiscardPile = true;
                        break;
                    }
                }

                if (!inDiscardPile)
                    return {};

                QList<SkillInvokeDetail> d;
                foreach (ServerPlayer *p, room->getAllPlayers()) {
                    if (p != move.from && p->hasSkill(this))
                        d << SkillInvokeDetail(this, p, p);
                }

                return d;
            }
        }

        return {};
    }

    bool cost(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &data) const override
    {
        CardsMoveOneTimeStruct move = data.value<CardsMoveOneTimeStruct>();
        QList<int> discardIds;
        foreach (int cardid, move.card_ids) {
            if (room->getCardPlace(cardid) == Player::DiscardPile)
                discardIds << cardid;
        }

        liaoguTempMove(room, invoke->invoker, discardIds);

        if (!room->askForUseCard(invoke->invoker, "@@liaogu", "@liaogu-use"))
            liaoguTempMove(room, invoke->invoker, {});

        return false;
    }

    bool effect(TriggerEvent, Room *, QSharedPointer<SkillInvokeDetail>, QVariant &) const override
    {
        return false;
    }
};

LiaoguCard::LiaoguCard()
{
    will_throw = false;
}

bool LiaoguCard::targetFixed(const Player *Self) const
{
    return Sanguosha->getCard(subcards.first())->targetFixed(Self);
}

bool LiaoguCard::targetFilter(const QList<const Player *> &targets, const Player *to_select, const Player *Self) const
{
    const Card *oc = Sanguosha->getCard(subcards.first());
    return oc->targetFilter(targets, to_select, Self) && !Self->isProhibited(to_select, oc, targets);
}

bool LiaoguCard::targetsFeasible(const QList<const Player *> &targets, const Player *Self) const
{
    const Card *oc = Sanguosha->getCard(subcards.first());
    if (oc->canRecast() && targets.length() == 0)
        return false;
    return oc->targetsFeasible(targets, Self);
}

void LiaoguCard::onUse(Room *room, const CardUseStruct &card_use) const
{
    Liaogu::liaoguTempMove(room, card_use.from, {});
    SkillCard::onUse(room, card_use);
}

void LiaoguCard::use(Room *room, const CardUseStruct &card_use) const
{
    CardUseStruct new_use = card_use;
    new_use.card = Sanguosha->getCard(subcards.first());
    room->useCard(new_use);

    if (!card_use.from->hasFlag("liaogulost") && card_use.from->canDiscard(card_use.from, "hes"))
        room->askForDiscard(card_use.from, "liaogu", 1, 1, false, true, "@liaogu-discard");
}

class Yvshou : public TriggerSkill
{
public:
    Yvshou()
        : TriggerSkill("yvshou")
    {
        events << CardUsed << CardResponded << EventPhaseStart << TurnStart;
    }

    void record(TriggerEvent triggerEvent, Room *room, QVariant &) const override
    {
        if (triggerEvent == TurnStart) {
            foreach (ServerPlayer *p, room->getAllPlayers()) {
                p->setMark("yvshou", 0);
                p->setFlags("-yvshouFirst");
            }
        }
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent triggerEvent, const Room *room, const QVariant &data) const override
    {
        QList<SkillInvokeDetail> r;

        if (triggerEvent == EventPhaseStart) {
            ServerPlayer *current = data.value<ServerPlayer *>();
            if (current->isAlive() && current->getPhase() == Player::Start) {
                foreach (ServerPlayer *p, room->getOtherPlayers(current)) {
                    if (p->isAlive() && p->hasSkill(this) && !p->isKongcheng())
                        r << SkillInvokeDetail(this, p, p, current);
                }
            }
        } else if ((triggerEvent == CardUsed) || (triggerEvent == CardResponded)) {
            const Card *card = nullptr;
            ServerPlayer *from = nullptr;
            if (triggerEvent == CardUsed) {
                CardUseStruct use = data.value<CardUseStruct>();
                card = use.card;
                from = use.from;
            } else {
                CardResponseStruct resp = data.value<CardResponseStruct>();
                if (resp.m_isUse) {
                    card = resp.m_card;
                    from = resp.m_from;
                }
            }

            if (card != nullptr && card->getNumber() > 0 && (!card->isVirtualCard() || card->getSubcards().length() == 1)
                && ((card->getTypeId() == Card::TypeBasic) || card->isNDTrick()) && from != nullptr && from->isInMainPhase() && !from->hasFlag("yvshouFirst")) {
                SkillInvokeDetail d(this, nullptr, nullptr, from, true);
                d.tag["yvshou"] = QVariant::fromValue<const Card *>(card);
                foreach (ServerPlayer *p, room->getOtherPlayers(from)) {
                    if (p->isAlive() && p->getMark("yvshou") > 0 && p->hasSkill(this)) {
                        d.invoker = p;
                        d.owner = p;
                        r << d;
                    }
                }
            }
        }

        return r;
    }

    bool cost(TriggerEvent triggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &data) const override
    {
        if (triggerEvent == EventPhaseStart) {
            const Card *c = room->askForCard(invoke->invoker, "..", "@yvshou-discard:" + invoke->targets.first()->objectName(), data, "yvshou");
            if (c != nullptr) {
                invoke->tag["yvshou"] = c->getEffectiveId();
                return true;
            }
        } else {
            invoke->targets.first()->setFlags("yvshouFirst");
            return TriggerSkill::cost(triggerEvent, room, invoke, data);
        }

        return false;
    }

    bool effect(TriggerEvent triggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &data) const override
    {
        if (triggerEvent == EventPhaseStart) {
            invoke->invoker->setMark("yvshou", Sanguosha->getCard(invoke->tag["yvshou"].toInt())->getNumber());
        } else {
            int mark = invoke->invoker->getMark("yvshou");
            const Card *c = invoke->tag.value("yvshou").value<const Card *>();
            int number = c->getNumber();

            LogMessage l;
            l.from = invoke->targets.first();
            l.arg = objectName();
            l.arg2 = c->objectName();

            if (number <= mark) {
                l.type = "#shenwei";
                if (triggerEvent == CardUsed) {
                    CardUseStruct use = data.value<CardUseStruct>();
                    if (use.card->isKindOf("Nullification"))
                        room->setPlayerFlag(use.from, "nullifiationNul");
                    else
                        use.nullified_list << "_ALL_TARGETS";
                    data = QVariant::fromValue<CardUseStruct>(use);
                } else {
                    CardResponseStruct resp = data.value<CardResponseStruct>();
                    resp.m_isNullified = true;
                    data = QVariant::fromValue<CardResponseStruct>(resp);
                }
            } else {
                l.type = "#fuchou";
                if (triggerEvent == CardUsed) {
                    CardUseStruct use = data.value<CardUseStruct>();
                    if (use.m_addHistory) {
                        room->addPlayerHistory(invoke->targets.first(), c->getClassName(), -1);
                        use.m_addHistory = false;
                        data = QVariant::fromValue<CardUseStruct>(use);
                    }
                }
            }

            room->sendLog(l);
        }

        return false;
    }
};

class LingduVS : public OneCardViewAsSkill
{
public:
    LingduVS()
        : OneCardViewAsSkill("lingdu")
    {
        expand_pile = "#judging_area";
        response_pattern = "@@lingdu";
    }

    bool viewFilter(const Card *) const override
    {
        return true;
    }

    const Card *viewAs(const Card *originalCard) const override
    {
        return new DummyCard({originalCard->getEffectiveId()});
    }
};

class Lingdu : public TriggerSkill
{
public:
    Lingdu()
        : TriggerSkill("lingdu")
    {
        events << CardsMoveOneTime;
        view_as_skill = new LingduVS;
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent, const Room *room, const QVariant &data) const override
    {
        ServerPlayer *current = room->getCurrent();
        if (current == nullptr || current->isDead() || current->getPhase() == Player::NotActive || current->hasFlag("lingdu"))
            return {};

        // move.from is filled with card user if the move is because of Use or Response.
        CardsMoveOneTimeStruct move = data.value<CardsMoveOneTimeStruct>();
        if (move.from == nullptr || !move.from->hasSkill(this) || move.from->isDead() || (move.to_place != Player::DiscardPile))
            return {};

        QList<int> card_ids;

        for (int i = 0; i < move.card_ids.length(); ++i) {
            if (room->getCardPlace(move.card_ids.at(i)) != Player::DiscardPile)
                continue;

            Player::Place fromPlace = move.from_places.at(i);
            switch (fromPlace) {
            case Player::PlaceHand:
            case Player::PlaceEquip:
            case Player::PlaceDelayedTrick: {
                card_ids << move.card_ids.at(i);
                break;
            }
            case Player::PlaceTable: {
                if (((move.reason.m_reason & CardMoveReason::S_MASK_BASIC_REASON) == CardMoveReason::S_REASON_USE)
                    || ((move.reason.m_reason & CardMoveReason::S_MASK_BASIC_REASON) == CardMoveReason::S_REASON_RESPONSE))
                    card_ids << move.card_ids.at(i);
                break;
            }
            default: {
                break;
            }
            }
        }

        if (card_ids.isEmpty())
            return {};

        ServerPlayer *from = qobject_cast<ServerPlayer *>(move.from);
        if (from == nullptr || from->getPhase() != Player::NotActive)
            return {};

        SkillInvokeDetail d(this, from, from);
        d.tag["lingdu"] = IntList2VariantList(card_ids);
        return {d};
    }

    bool cost(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const override
    {
        const Card *l = room->askForCard(invoke->invoker, "@@lingdu", "@lingdu-discard", invoke->tag.value("lingdu"), Card::MethodNone, nullptr, false, "lingdu");
        if (l != nullptr) {
            // need #InvokeSkill log? waiting for test result

            room->getCurrent()->setFlags("lingdu");
            QList<int> card_ids = VariantList2IntList(invoke->tag.value("lingdu").toList());
            int id = -1;
            if (card_ids.length() == 1)
                id = card_ids.first();
            else {
                room->fillAG(card_ids, invoke->invoker);
                id = room->askForAG(invoke->invoker, card_ids, false, "lingdu");
                room->clearAG(invoke->invoker);
                if (id == -1)
                    id = card_ids.first();
            }

            bool isLastCard = false;
            switch (room->getCardPlace(l->getEffectiveId())) {
            case Player::PlaceHand: {
                isLastCard = (invoke->invoker->getHandcardNum() == 1 && invoke->invoker->getHandcards().first()->getEffectiveId() == l->getEffectiveId());
                break;
            }
            case Player::PlaceEquip: {
                isLastCard = (invoke->invoker->getEquips().length() == 1 && invoke->invoker->getEquips().first()->getEffectiveId() == l->getEffectiveId());
                break;
            }
            case Player::PlaceDelayedTrick: {
                isLastCard = (invoke->invoker->getJudgingAreaID().length() == 1 && invoke->invoker->getJudgingAreaID().first() == l->getEffectiveId());
                break;
            }
            default: {
                break;
            }
            }

            int card1 = l->getEffectiveId();
            int card2 = id;
            invoke->tag["isLast"] = isLastCard;

            LogMessage l;
            l.type = "$lingdulost";
            l.from = invoke->invoker;
            l.card_str = QString::number(card1);
            l.arg = objectName();
            room->sendLog(l);

            CardsMoveStruct move1;
            move1.card_ids << card1;
            move1.to = nullptr;
            move1.to_place = Player::DiscardPile;
            move1.reason.m_reason = CardMoveReason::S_REASON_PUT;
            move1.reason.m_playerId = invoke->invoker->objectName();
            move1.reason.m_skillName = "lingdu";
            CardsMoveStruct move2;
            move2.card_ids << card2;
            move2.to = invoke->invoker;
            move2.to_place = Player::PlaceHand;
            move2.reason.m_reason = CardMoveReason::S_REASON_RECYCLE;
            move2.reason.m_playerId = invoke->invoker->objectName();
            move2.reason.m_skillName = "lingdu";

            room->moveCardsAtomic({move1, move2}, true);

            return true;
        }

        return false;
    }

    bool effect(TriggerEvent /*triggerEvent*/, Room * /*room*/, QSharedPointer<SkillInvokeDetail> invoke, QVariant & /*data*/) const override
    {
        if (invoke->tag.value("isLast").toBool())
            invoke->invoker->drawCards(1, "lingdu");

        return false;
    }
};

class Duozhi : public TriggerSkill
{
public:
    Duozhi()
        : TriggerSkill("duozhi")
    {
        events << CardFinished << EventPhaseStart;
        frequency = Compulsory;
    }

    void record(TriggerEvent triggerEvent, Room *room, QVariant &data) const override
    {
        if (triggerEvent == EventPhaseStart) {
            ServerPlayer *c = data.value<ServerPlayer *>();
            if (c->getPhase() == Player::NotActive) {
                foreach (ServerPlayer *p, room->getAllPlayers()) {
                    if (p->isCardLimited("use,response", "duozhi"))
                        room->removePlayerCardLimitation(p, "use,response", ".$1", "duozhi", true);
                }
            }
        }
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent triggerEvent, const Room *room, const QVariant &data) const override
    {
        if (triggerEvent == CardFinished) {
            CardUseStruct use = data.value<CardUseStruct>();
            if (use.from != nullptr && use.from->isAlive() && use.from->hasSkill(this)) {
                ServerPlayer *current = room->getCurrent();
                if (current != nullptr && current->getPhase() != Player::NotActive && current->isAlive()) {
                    foreach (ServerPlayer *p, room->getOtherPlayers(use.from)) {
                        if (!p->isCardLimited("use,response", "duozhi"))
                            return {SkillInvokeDetail(this, use.from, use.from, nullptr, true)};
                    }
                }
            }
        }

        return {};
    }

    bool cost(TriggerEvent triggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &data) const override
    {
        if (TriggerSkill::cost(triggerEvent, room, invoke, data)) {
            if (invoke->invoker->hasShownSkill(this)) {
                LogMessage l;
                l.type = "#TriggerSkill";
                l.from = invoke->invoker;
                l.arg = objectName();
                room->sendLog(l);
            }

            return true;
        }

        return false;
    }

    bool effect(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const override
    {
        LogMessage l;

        foreach (ServerPlayer *p, room->getOtherPlayers(invoke->invoker)) {
            if (!p->isCardLimited("use,response", "duozhi")) {
                room->setPlayerCardLimitation(p, "use,response", ".", "duozhi", true);
                l.to << p;
            }
        }

        if (!l.to.isEmpty()) {
            l.type = "#duozhi";
            room->sendLog(l);
        }

        return false;
    }
};

class LingjunOtherVS : public OneCardViewAsSkill
{
public:
    LingjunOtherVS()
        : OneCardViewAsSkill("LingjunOtherVS")
    {
        response_or_use = true;
        response_pattern = "@@LingjunOtherVS";
        filter_pattern = "BasicCard";
    }

    const Card *viewAs(const Card *originalCard) const override
    {
        Slash *s = new Slash(Card::SuitToBeDecided, -1);
        s->addSubcard(originalCard);
        s->setSkillName("_lingjun");
        return s;
    }
};

class Lingjun : public TriggerSkill
{
private:
    static const Card *askForUseLingjunTo(Room *room, ServerPlayer *invoker, ServerPlayer *slasher, ServerPlayer *victim)
    {
        // copy from Room::askForUseSlashTo
        room->setPlayerFlag(slasher, "slashTargetFix");
        room->setPlayerFlag(slasher, "slashNoDistanceLimit");
        room->setPlayerFlag(slasher, "slashDisableExtraTarget");
        room->setPlayerFlag(slasher, "slashTargetFixToOne");
        room->setPlayerFlag(victim, "SlashAssignee");
        room->setPlayerFlag(slasher, "SlashRecorder_Lingjun_" + invoker->objectName());

        const Card *slash = room->askForUseCard(slasher, "@@LingjunOtherVS", "@lingjun-fire:" + invoker->objectName() + ":" + victim->objectName(), -1, Card::MethodUse, false);
        if (slash == nullptr) {
            room->setPlayerFlag(slasher, "-slashTargetFix");
            room->setPlayerFlag(slasher, "-slashTargetFixToOne");
            room->setPlayerFlag(victim, "-SlashAssignee");
            if (slasher->hasFlag("slashNoDistanceLimit"))
                room->setPlayerFlag(slasher, "-slashNoDistanceLimit");
            if (slasher->hasFlag("slashDisableExtraTarget"))
                room->setPlayerFlag(slasher, "-slashDisableExtraTarget");
            room->setPlayerFlag(slasher, "-SlashRecorder_Lingjun_" + invoker->objectName());
        }

        return slash;
    }

public:
    Lingjun()
        : TriggerSkill("lingjun")
    {
        events << PreCardUsed << CardFinished << TurnStart;
        global = true;
    }

    void record(TriggerEvent triggerEvent, Room *room, QVariant &data) const override
    {
        if (triggerEvent == PreCardUsed) {
            CardUseStruct use = data.value<CardUseStruct>();
            if (use.card->isKindOf("Slash")) {
                if (use.from != nullptr && !use.from->hasFlag("lingjun_firstslash")) {
                    use.card->setFlags("lingjun_firstslash");
                    use.from->setFlags("lingjun_firstslash");
                }

                if (use.card->getSkillName() == "lingjun" && !use.card->getSubcards().isEmpty() && !Sanguosha->getCard(use.card->getSubcards().first())->isKindOf("Slash")) {
                    QStringList flags = use.card->getFlags();
                    foreach (const QString &flag, flags) {
                        if (flag.startsWith("Lingjun_")) {
                            QString s = flag.mid(8);
                            ServerPlayer *lingjunInvoker = room->findPlayerByObjectName(s);
                            if (lingjunInvoker != nullptr)
                                lingjunInvoker->setFlags("lingjunNotSlash");
                        }
                    }
                }
            }
        } else if (triggerEvent == TurnStart) {
            foreach (ServerPlayer *p, room->getAllPlayers()) {
                p->setFlags("-lingjun_firstslash");
                p->setFlags("-lingjunNotSlash");
            }
        }
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent triggerEvent, const Room *room, const QVariant &data) const override
    {
        if (triggerEvent == CardFinished) {
            CardUseStruct use = data.value<CardUseStruct>();
            if (use.from != nullptr && use.from->isAlive() && use.from->hasSkill(this) && use.card->hasFlag("lingjun_firstslash") && !use.to.isEmpty()) {
                QList<ServerPlayer *> ps;
                foreach (ServerPlayer *to, use.to) {
                    foreach (ServerPlayer *p, room->getOtherPlayers(use.from)) {
                        if (p->inMyAttackRange(to)) {
                            ps << to;
                            break;
                        }
                    }
                }

                if (ps.isEmpty())
                    return {};

                return {SkillInvokeDetail(this, use.from, use.from)};
            }
        }
        return {};
    }

    bool cost(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &data) const override
    {
        CardUseStruct use = data.value<CardUseStruct>();
        QList<ServerPlayer *> ps;
        foreach (ServerPlayer *to, use.to) {
            foreach (ServerPlayer *p, room->getOtherPlayers(invoke->invoker)) {
                if (p->inMyAttackRange(to)) {
                    ps << to;
                    break;
                }
            }
        }

        if (ps.isEmpty())
            return false;

        ServerPlayer *victim = room->askForPlayerChosen(invoke->invoker, ps, "lingjun", "@lingjun-concentratefire", true, true);

        if (victim != nullptr) {
            invoke->targets << victim;
            return true;
        }

        return false;
    }

    bool effect(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const override
    {
        bool flag = false;

        foreach (ServerPlayer *p, room->getOtherPlayers(invoke->invoker)) {
            if (invoke->targets.first()->isAlive() && p->inMyAttackRange(invoke->targets.first())) {
                if (askForUseLingjunTo(room, invoke->invoker, p, invoke->targets.first()) != nullptr)
                    flag = true;
            }
        }

        if (flag && !invoke->invoker->hasFlag("lingjunNotSlash") && invoke->targets.first()->isAlive()) {
            Slash *s = new Slash(Card::NoSuit, 0);
            s->setSkillName("_lingjun");
            if (!invoke->invoker->isLocked(s))
                room->useCard(CardUseStruct(s, invoke->invoker, invoke->targets.first()));
            else
                delete s;
        }

        return false;
    }
};

class Ciou : public TriggerSkill
{
public:
    Ciou()
        : TriggerSkill("ciou")
    {
        events << DamageInflicted << DamageComplete;
        frequency = Compulsory;
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent triggerEvent, const Room *, const QVariant &data) const override
    {
        DamageStruct damage = data.value<DamageStruct>();
        if (triggerEvent == DamageInflicted && (damage.to->isAlive() && damage.to->hasSkill(this)))
            return {SkillInvokeDetail(this, damage.to, damage.to, nullptr, true)};
        else if (triggerEvent == DamageComplete && damage.trigger_info.contains("ciou_destructing") && damage.to->isAlive())
            return {SkillInvokeDetail(this, damage.to, damage.to, nullptr, true, nullptr, false)};

        return {};
    }

    bool effect(TriggerEvent triggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &data) const override
    {
        DamageStruct damage = data.value<DamageStruct>();
        LogMessage l;
        l.from = invoke->invoker;
        l.arg = objectName();

        if (triggerEvent == DamageInflicted) {
            if (damage.nature == DamageStruct::Normal && damage.card != nullptr && damage.card->isKindOf("Slash")) {
                l.type = "#TriggerSkill";
                damage.trigger_info.append("ciou_destructing");
            } else {
                l.type = "#micai01";
                l.arg2 = QString::number(1);
                damage.damage -= 1;
            }
            room->sendLog(l);
            room->notifySkillInvoked(invoke->invoker, "ciou");
            data = QVariant::fromValue<DamageStruct>(damage);
            return damage.damage == 0;
        } else if (triggerEvent == DamageComplete) {
            l.type = "#ciou";
            room->sendLog(l);
            room->loseHp(invoke->invoker);
        }

        return false;
    }
};

class JinjiD : public DistanceSkill
{
public:
    JinjiD()
        : DistanceSkill("#jinji")
    {
    }

    int getCorrect(const Player *from, const Player *to) const override
    {
        if (from == to)
            return 0;
        int i = 0;
        if (from->hasSkill("jinji"))
            i -= 1;
        if (to->hasSkill("jinji"))
            i += 1;

        return i;
    }
};


class Jinji : public TriggerSkill
{
public:
    Jinji()
        : TriggerSkill("jinji")
    {
        events << CardResponded << CardUsed << EventPhaseChanging;
        frequency = Compulsory;
    }

    void record(TriggerEvent e, Room *room, QVariant &data) const override
    {
        if (e == EventPhaseChanging) {
            PhaseChangeStruct change = data.value<PhaseChangeStruct>();
            if (change.to == Player::NotActive) {
                foreach(ServerPlayer *p, room->getAllPlayers()) {
                    if (p->getMark("jinji_invalid") >0) {
                        room->setPlayerMark(p, "jinji_invalid", 0);
                        room->setPlayerSkillInvalidity(p, objectName(), false);
                    }
                }
            }
        }
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent triggerEvent, const Room *, const QVariant &data) const override
    {
        ServerPlayer *user = nullptr;
        const Card *card = nullptr;
        if (triggerEvent == CardUsed) {
            CardUseStruct use = data.value<CardUseStruct>();
            card = use.card;
            user = use.from;
        }
        else if (triggerEvent == CardResponded) {
            CardResponseStruct resp = data.value<CardResponseStruct>();
            card = resp.m_card;
            user = resp.m_from;
        }

        if (user != nullptr && user->hasSkill(this) && user->isAlive() && card != nullptr && card->getTypeId() == Card::TypeBasic )
            return QList<SkillInvokeDetail>() << SkillInvokeDetail(this, user, user, nullptr, true);

        return QList<SkillInvokeDetail>();
    }

    bool effect(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const override
    {
        room->touhouLogmessage("#TriggerSkill", invoke->invoker, objectName());
        room->setPlayerMark(invoke->invoker, "jinji_invalid", 1);
        room->setPlayerSkillInvalidity(invoke->invoker, objectName(), true);

        return false;
    }
};

class TianxingVS : public ZeroCardViewAsSkill
{
public:
    TianxingVS()
        : ZeroCardViewAsSkill("tianxing")
    {
        response_pattern = "@@tianxing";
    }


    const Card *viewAs() const override
    {
        PowerSlash *card = new PowerSlash(Card::SuitToBeDecided, -1);
        card->setSkillName("tianxing");
        return card;
    }
};

class Tianxing : public TriggerSkill
{
public:
    Tianxing()
        : TriggerSkill("tianxing")
    {
        events << EventPhaseStart << EventPhaseEnd;
        view_as_skill = new TianxingVS;
        global = true;//need global?
    }

    void record(TriggerEvent triggerEvent, Room *room, QVariant &data) const override
    {
        //updatemark 
        if (triggerEvent == EventPhaseStart) {
            ServerPlayer *c = data.value<ServerPlayer *>();
            if (c != nullptr && c->getPhase() == Player::Play) {
                foreach(ServerPlayer *p, room->getAllPlayers()) {
                    int m = 0;
                    foreach(ServerPlayer *t, room->getOtherPlayers(p)) {
                        if (p->inMyAttackRange(t))
                            m++;
                    }
                    room->setPlayerMark(p, "tianxing", m);
                }
            }
        }
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent triggerEvent, const Room *room, const QVariant &data) const override
    {
        QList<SkillInvokeDetail> r;

        if (triggerEvent == EventPhaseEnd) {
            ServerPlayer *c = data.value<ServerPlayer *>();
            if (c !=nullptr && c->getPhase() == Player::Play) { //need check alive?
                foreach(ServerPlayer *p, room->findPlayersBySkillName(objectName())) {
                    if (p->isAlive()) {
                        int m = 0;
                        foreach(ServerPlayer *t, room->getOtherPlayers(p)) {
                            if (p->inMyAttackRange(t))
                                m++;
                        }
                        if (m != p->getMark("tianxing"))
                            r << SkillInvokeDetail(this, p, p);
                    }
                }
            }
        }

        return r;
    }

    bool cost(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const override
    {
        room->askForUseCard(invoke->invoker, "@@tianxing", "@tianxing");
        return false;
    }

};

class TianxingTargetMod : public TargetModSkill
{
public:
    TianxingTargetMod()
        : TargetModSkill("#tianxing-targetmod")
    {
        pattern = "Slash";
    }

    int getDistanceLimit(const Player *, const Card *c) const override
    {
        if (c->getSkillName()=="tianxing")
            return 1000;

        return 0;
    }
};
/*
class TianxingVS : public OneCardViewAsSkill
{
public:
    TianxingVS()
        : OneCardViewAsSkill("tianxing")
    {
        response_pattern = "@@tianxing";
    }

    bool viewFilter(const Card *to_select) const override
    {
        return Self->hasEquip(to_select) && !Self->isBrokenEquip(to_select->getEffectiveId());
    }

    const Card *viewAs(const Card *originalCard) const override
    {
        return new DummyCard({originalCard->getEffectiveId()});
    }
};

class Tianxing : public TriggerSkill
{
public:
    Tianxing()
        : TriggerSkill("tianxing")
    {
        events << Damage << EventPhaseStart;
        view_as_skill = new TianxingVS;
    }

    void record(TriggerEvent triggerEvent, Room *room, QVariant &data) const override
    {
        if (triggerEvent == EventPhaseStart) {
            ServerPlayer *c = data.value<ServerPlayer *>();
            if (c->getPhase() == Player::NotActive) {
                foreach (ServerPlayer *p, room->getAllPlayers()) {
                    if (p->getMark("tianxing") > 0) {
                        p->setMark("tianxing", 0);
                        room->setPlayerProperty(p, "tianxing", QString());
                    }
                }
            }
        }
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent triggerEvent, const Room *room, const QVariant &data) const override
    {
        QList<SkillInvokeDetail> r;

        if (triggerEvent == EventPhaseStart) {
            ServerPlayer *c = data.value<ServerPlayer *>();
            if (c->getPhase() == Player::Start) {
                foreach (ServerPlayer *p, room->findPlayersBySkillName(objectName())) {
                    if (p->isAlive() && p->canSlash(c, false)) {
                        bool flag = false;
                        foreach (const Card *c, p->getEquips()) {
                            if (!p->getBrokenEquips().contains(c->getEffectiveId())) {
                                flag = true;
                                break;
                            }
                        }
                        if (flag)
                            r << SkillInvokeDetail(this, p, p, c);
                    }
                }
            }
        } else {
            DamageStruct damage = data.value<DamageStruct>();
            if (damage.from != nullptr && damage.from->isAlive() && damage.from->hasSkill(this) && damage.card != nullptr && damage.card->isKindOf("Slash")
                && !damage.to->property("tianxing").toString().split("+").contains(damage.from->objectName()))
                r << SkillInvokeDetail(this, damage.from, damage.from, damage.to, true);
        }

        return r;
    }

    bool cost(TriggerEvent triggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &data) const override
    {
        if (triggerEvent == EventPhaseStart) {
            const Card *c = room->askForCard(invoke->invoker, "@@tianxing", "@tianxing-discard", data, Card::MethodNone, nullptr, false, "tianxing");
            if (c != nullptr) {
                invoke->invoker->addBrokenEquips({c->getEffectiveId()});
                return true;
            }
        } else {
            if (TriggerSkill::cost(triggerEvent, room, invoke, data)) {
                if (invoke->invoker->hasShownSkill(this)) {
                    LogMessage l;
                    l.type = "#TriggerSkill";
                    l.from = invoke->invoker;
                    l.arg = objectName();
                    room->sendLog(l);
                }

                return true;
            }
        }

        return false;
    }

    bool effect(TriggerEvent triggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const override
    {
        if (triggerEvent == EventPhaseStart) {
            Slash *s = new Slash(Card::NoSuit, 0);
            s->setSkillName("_tianxing");
            if (!invoke->invoker->isLocked(s))
                room->useCard(CardUseStruct(s, invoke->invoker, invoke->targets.first()));
            else
                delete s;
        } else {
            invoke->targets.first()->setMark("tianxing", 1);

            LogMessage l;
            l.type = "#tianxing";
            l.from = invoke->targets.first();
            l.to << invoke->invoker;
            room->sendLog(l);

            QSet<QString> prohibited = invoke->targets.first()->property("tianxing").toString().split("+").toSet();
            prohibited << invoke->invoker->objectName();
            room->setPlayerProperty(invoke->targets.first(), "tianxing", QStringList(prohibited.toList()).join("+"));
        }

        return false;
    }
};

class TianxingP : public ProhibitSkill
{
public:
    TianxingP()
        : ProhibitSkill("#tianxing-prohibit")
    {
    }

    bool isProhibited(const Player *from, const Player *to, const Card *card, const QList<const Player *> &others, bool) const override
    {
        return from != nullptr && from->property("tianxing").toString().split("+").contains(to->objectName()) && card->getTypeId() != Card::TypeSkill && others.isEmpty();
    }
};*/

TH17Package::TH17Package()
    : Package("th17")
{
    General *keiki = new General(this, "keiki$", "gxs");
    keiki->addSkill(new Shanxing);
    keiki->addSkill(new Lingshou);
    keiki->addSkill(new Qijue);
    //keiki->addSkill(new Zhuying);
    //keiki->addSkill(new ZhuyingD);
    //related_skills.insertMulti("zhuying", "#zhuying-distance");

    General *saki = new General(this, "saki", "gxs");
    saki->addSkill(new Jinji);
    saki->addSkill(new JinjiD);
    saki->addSkill(new Tianxing);
    saki->addSkill(new TianxingTargetMod);
    //saki->addSkill(new TianxingP);
    related_skills.insertMulti("jinji", "#jinji");
    related_skills.insertMulti("tianxing", "#tianxing-targetmod");

    General *mayumi = new General(this, "mayumi", "gxs");
    mayumi->addSkill(new Lingjun);
    mayumi->addSkill(new Ciou);

    General *yachie = new General(this, "yachie", "gxs");
    yachie->addSkill(new Duozhi);

    General *kutaka = new General(this, "kutaka", "gxs");
    kutaka->addSkill(new Yvshou);
    kutaka->addSkill(new Lingdu);

    General *urumi = new General(this, "urumi", "gxs");
    urumi->addSkill(new Lunni);
    urumi->addSkill(new Liaogu);

    General *eika = new General(this, "eika", "gxs", 3);
    eika->addSkill(new Shanlei);
    eika->addSkill(new Bengluo);

    addMetaObject<LunniCard>();
    //addMetaObject<ZhuyingCard>();

    skills << new LingjunOtherVS << new LingshouOtherVS; //<< new ZhuyingAttach;
}

ADD_PACKAGE(TH17)
