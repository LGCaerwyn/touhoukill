#include "th99.h"
#include "general.h"
#include "skill.h"
#include "engine.h"
#include "standard.h"
#include "client.h"
#include "maneuvering.h"



QiuwenCard::QiuwenCard()
{
    mute = true;
    target_fixed = true;
}

void QiuwenCard::use(Room *room, ServerPlayer *source, QList<ServerPlayer *> &) const
{
    room->drawCards(source, 3);
}

class Qiuwen : public ZeroCardViewAsSkill
{
public:
    Qiuwen() : ZeroCardViewAsSkill("qiuwen")
    {
    }

    virtual bool isEnabledAtPlay(const Player *player) const
    {
        return !player->hasUsed("QiuwenCard");
    }

    virtual const Card *viewAs() const
    {
        return new QiuwenCard;
    }
};

class Zaozu : public TriggerSkill
{
public:
    Zaozu() : TriggerSkill("zaozu")
    {
        events << EventPhaseChanging << EventPhaseStart;
        frequency = Compulsory;
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent triggerEvent, const Room *, const QVariant &data) const
    {
        if (triggerEvent == EventPhaseChanging) {
            PhaseChangeStruct change = data.value<PhaseChangeStruct>();
            if (change.player->isAlive() && change.player->hasSkill(this) && change.to == Player::Discard)
                return QList<SkillInvokeDetail>() << SkillInvokeDetail(this, change.player, change.player, NULL, true);
        } else if (triggerEvent == EventPhaseStart) {
            ServerPlayer *aq = data.value<ServerPlayer *>();
            if (aq->isAlive() && aq->hasSkill(this) && aq->getPhase() == Player::Finish && aq->getHandcardNum() > aq->getMaxHp())
                return QList<SkillInvokeDetail>() << SkillInvokeDetail(this, aq, aq, NULL, true);
        }

        return QList<SkillInvokeDetail>();
    }

    bool effect(TriggerEvent triggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const
    {
        room->notifySkillInvoked(invoke->invoker, objectName());
        room->touhouLogmessage("#TriggerSkill", invoke->invoker, objectName());
        if (triggerEvent == EventPhaseChanging)
            invoke->invoker->skip(Player::Discard);
        else if (triggerEvent == EventPhaseStart)
            room->loseHp(invoke->invoker);

        return false;
    }
};

DangjiaCard::DangjiaCard()
{
    will_throw = false;
    handling_method = Card::MethodNone;
    m_skillName = "dangjia_attach";
}

bool DangjiaCard::targetFilter(const QList<const Player *> &targets, const Player *to_select, const Player *Self) const
{
    return targets.isEmpty() && to_select->hasLordSkill("dangjia")
        && to_select != Self && !to_select->hasFlag("dangjiaInvoked")
        && !to_select->isKongcheng() && to_select->isWounded();
}

void DangjiaCard::use(Room *room, ServerPlayer *source, QList<ServerPlayer *> &targets) const
{
    ServerPlayer *akyu = targets.first();
    if (akyu->hasLordSkill("dangjia")) {
        room->setPlayerFlag(akyu, "dangjiaInvoked");

        room->notifySkillInvoked(akyu, "dangjia");
        if (!source->pindian(akyu, "dangjia", NULL)) {
            RecoverStruct recov;
            recov.who = source;
            room->recover(akyu, recov);
        }

        QList<ServerPlayer *> akyus;
        QList<ServerPlayer *> players = room->getOtherPlayers(source);
        foreach (ServerPlayer *p, players) {
            if (p->hasLordSkill("dangjia") && !p->hasFlag("dangjiaInvoked"))
                akyus << p;
        }
        if (akyus.isEmpty())
            room->setPlayerFlag(source, "Forbiddangjia");
    }
}

class DangjiaVS : public ZeroCardViewAsSkill
{
public:
    DangjiaVS() :ZeroCardViewAsSkill("dangjia_attach")
    {
        attached_lord_skill = true;
    }

    virtual bool isEnabledAtPlay(const Player *player) const
    {
        return  shouldBeVisible(player) && !player->hasFlag("Forbiddangjia") && !player->isKongcheng();
    }

    virtual bool shouldBeVisible(const Player *Self) const
    {
        return Self && Self->getKingdom() == "wai";
    }

    virtual const Card *viewAs() const
    {
        return new DangjiaCard;
    }
};

class Dangjia : public TriggerSkill
{
public:
    Dangjia() : TriggerSkill("dangjia$")
    {
        events << GameStart << EventAcquireSkill << EventLoseSkill << EventPhaseChanging;
    }


    void record(TriggerEvent triggerEvent, Room *room, QVariant &data) const
    {
        if (triggerEvent != EventPhaseChanging) {
            static QString attachName = "dangjia_attach";
            QList<ServerPlayer *> aqs;
            foreach (ServerPlayer *p, room->getAllPlayers()) {
                if (p->hasLordSkill(this, true))
                    aqs << p;
            }

            if (aqs.length() > 1) {
                foreach (ServerPlayer *p, room->getAllPlayers()) {
                    if (!p->hasLordSkill(attachName, true))
                        room->attachSkillToPlayer(p, attachName);
                }
            } else if (aqs.length() == 1) {
                foreach (ServerPlayer *p, room->getAllPlayers()) {
                    if (p->hasLordSkill(this, true) && p->hasLordSkill(attachName, true))
                        room->detachSkillFromPlayer(p, attachName, true);
                    else if (!p->hasLordSkill(this, true) && !p->hasLordSkill(attachName, true))
                        room->attachSkillToPlayer(p, attachName);
                }
            } else { // the case that aqs is empty
                foreach (ServerPlayer *p, room->getAllPlayers()) {
                    if (p->hasLordSkill(attachName, true))
                        room->detachSkillFromPlayer(p, attachName, true);
                }
            }
        } else {
            PhaseChangeStruct phase_change = data.value<PhaseChangeStruct>();
            if (phase_change.from != Player::Play)
                return;
            if (phase_change.player->hasFlag("Forbiddangjia"))
                room->setPlayerFlag(phase_change.player, "-Forbiddangjia");
            QList<ServerPlayer *> players = room->getOtherPlayers(phase_change.player);
            foreach (ServerPlayer *p, players) {
                if (p->hasFlag("dangjiaInvoked"))
                    room->setPlayerFlag(p, "-dangjiaInvoked");
            }
        }
    }
};

XiufuCard::XiufuCard()
{
    target_fixed = true;
}

bool XiufuCard::putToPile(Room *room, ServerPlayer *mori)
{
    QList<int> discardpile = room->getDiscardPile();
    QList<int> equips;
    foreach (int id, discardpile) {
        if (Sanguosha->getCard(id)->isKindOf("EquipCard"))
            equips << id;
    }

    if (equips.isEmpty())
        return false;

    CardsMoveStruct move;
    move.from_place = Player::DiscardPile;
    move.to = mori;
    move.to_player_name = mori->objectName();
    move.to_pile_name = "#xiufu_temp";
    move.card_ids = equips;
    move.to_place = Player::PlaceSpecial;
    move.open = true;
    move.card_ids = equips;

    QList<CardsMoveStruct> _moves = QList<CardsMoveStruct>() << move;
    QList<ServerPlayer *> _mori = QList<ServerPlayer *>() << mori;
    room->setPlayerFlag(mori, "xiufu_InTempMoving");
    room->notifyMoveCards(true, _moves, true, _mori);
    room->notifyMoveCards(false, _moves, true, _mori);
    room->setPlayerFlag(mori, "-xiufu_InTempMoving");

    QVariantList tag = IntList2VariantList(equips);
    mori->tag["xiufu_tempcards"] = tag;
    return true;
}

void XiufuCard::cleanUp(Room *room, ServerPlayer *mori)
{
    QList<int> equips = VariantList2IntList(mori->tag.value("xiufu_tempcards", QVariantList()).toList());
    mori->tag.remove("xiufu_tempcards");
    if (equips.isEmpty())
        return;

    CardsMoveStruct move;
    move.from = mori;
    move.from_player_name = mori->objectName();
    move.from_place = Player::PlaceSpecial;
    move.from_pile_name = "#xiufu_temp";
    move.to_place = Player::DiscardPile;
    move.open = true;
    move.card_ids = equips;

    QList<CardsMoveStruct> _moves = QList<CardsMoveStruct>() << move;
    QList<ServerPlayer *> _mori = QList<ServerPlayer *>() << mori;
    room->setPlayerFlag(mori, "xiufu_InTempMoving");
    room->notifyMoveCards(true, _moves, true, _mori);
    room->notifyMoveCards(false, _moves, true, _mori);
    room->setPlayerFlag(mori, "-xiufu_InTempMoving");
}

void XiufuCard::onUse(Room *room, const CardUseStruct &card_use) const
{
    ServerPlayer *mori = card_use.from;
    if (mori->isOnline()) {
        if (!putToPile(room, mori))
            return;

        bool used = room->askForUseCard(mori, "@@xiufumove", "@xiufu-move", -1, Card::MethodNone, true, "xiufu");

        cleanUp(room, mori);
        if (!used)
            return;
    } else {
        // we use askforag and askforplayerchosen for AI
        QList<int> discardpile = room->getDiscardPile();
        QList<int> equips;
        foreach (int id, discardpile) {
            if (Sanguosha->getCard(id)->isKindOf("EquipCard"))
                equips << id;
        }

        int id = room->askForAG(mori, equips, true, "xiufu");
        if (id == -1)
            return;

        ServerPlayer *target = room->askForPlayerChosen(mori, room->getAllPlayers(), "xiufu", QString(), true);
        if (target == NULL)
            return;

        mori->tag["xiufu_id"] = id;
        mori->tag["xiufu_to"] = QVariant::fromValue(target);
    }

    SkillCard::onUse(room, card_use);
}

void XiufuCard::use(Room *room, ServerPlayer *mori, QList<ServerPlayer *> &) const
{
    //process move
    int xiufu_id = mori->tag.value("xiufu_id", -1).toInt();
    if (xiufu_id == -1)
        return;

    const EquipCard *equip = qobject_cast<const EquipCard *>(Sanguosha->getCard(xiufu_id)->getRealCard());
    ServerPlayer *target = mori->tag.value("xiufu_to", QVariant::fromValue((ServerPlayer *)(NULL))).value<ServerPlayer *>();

    int equipped_id = -1;
    if (target->getEquip(equip->location()) != NULL)
        equipped_id = target->getEquip(equip->location())->getEffectiveId();
    QList<CardsMoveStruct> exchangeMove;
    CardsMoveStruct move1(equip->getId(), target, Player::PlaceEquip, CardMoveReason(CardMoveReason::S_REASON_TRANSFER, mori->objectName()));
    exchangeMove.push_back(move1);
    if (equipped_id != -1) {
        CardsMoveStruct move2(equipped_id, NULL, Player::DiscardPile, CardMoveReason(CardMoveReason::S_REASON_CHANGE_EQUIP, target->objectName()));
        exchangeMove.push_back(move2);
    }

    LogMessage zhijian;
    zhijian.type = "$ZhijianEquip";
    zhijian.from = target;
    zhijian.card_str = QString::number(equip->getId());
    room->sendLog(zhijian);

    room->moveCardsAtomic(exchangeMove, true);
}

XiufuMoveCard::XiufuMoveCard()
{
    will_throw = false;
    handling_method = Card::MethodNone;
}

bool XiufuMoveCard::targetFilter(const QList<const Player *> &targets, const Player *, const Player *) const
{
    return targets.isEmpty();
}

void XiufuMoveCard::onUse(Room *, const CardUseStruct &card_use) const
{
    card_use.from->tag["xiufu_id"] = subcards.first();
    card_use.from->tag["xiufu_to"] = QVariant::fromValue(card_use.to.first());
}

class XiufuMove : public OneCardViewAsSkill
{
public:
    XiufuMove() : OneCardViewAsSkill("xiufumove")
    {
        response_pattern = "@@xiufumove";
        expand_pile = "#xiufu_temp";
    }

    bool viewFilter(const Card *to_select) const
    {
        return Self->getPile("#xiufu_temp").contains(to_select->getId());
    }

    const Card *viewAs(const Card *originalCard) const
    {
        XiufuMoveCard *move = new XiufuMoveCard;
        move->addSubcard(originalCard);
        return move;
    }
};

class Xiufu : public ZeroCardViewAsSkill
{
public:
    Xiufu() : ZeroCardViewAsSkill("xiufu")
    {

    }

    bool isEnabledAtPlay(const Player *player) const
    {
        return !player->hasUsed("XiufuCard") || player->getMark("@xiufudebug") > 0;
    }

    const Card *viewAs() const
    {
        return new XiufuCard;
    }
};

class XiufuDebug : public TriggerSkill
{
public:
    XiufuDebug() : TriggerSkill("xiufu")
    {
        events << MarkChanged;
        view_as_skill = new Xiufu;
    }

    void record(TriggerEvent, Room *room, QVariant &data) const
    {
        MarkChangeStruct change = data.value<MarkChangeStruct>();
        if (change.name == "@xiufudebug") {
            QList<int> equips;
            foreach (int id, Sanguosha->getRandomCards()) {
                const Card *card = Sanguosha->getEngineCard(id);
                if (card->isKindOf("EquipCard") && room->getCardPlace(id) != Player::DiscardPile)
                    equips << id;
            }

            DummyCard dummy(equips);
            room->moveCardTo(&dummy, NULL, Player::DiscardPile);
        }
    }
};

class Fandu : public TriggerSkill
{
public:
    Fandu() : TriggerSkill("fandu")
    {
        events << EventPhaseStart << Damaged;
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent triggerEvent, const Room *, const QVariant &data) const
    {
        if (triggerEvent == EventPhaseStart) {
            ServerPlayer *p = data.value<ServerPlayer *>();
            if (p->isAlive() && p->hasSkill(this) && p->getPhase() == Player::Start)
                return QList<SkillInvokeDetail>() << SkillInvokeDetail(this, p, p);
        } else {
            DamageStruct damage = data.value<DamageStruct>();
            if (damage.to->isAlive() && damage.to->hasSkill(this)) {
                QList<SkillInvokeDetail> d;
                for (int i = 0; i < damage.damage; ++i)
                    d << SkillInvokeDetail(this, damage.to, damage.to);
                return d;
            }
        }

        return QList<SkillInvokeDetail>();
    }

    bool effect(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const
    {
        invoke->invoker->drawCards(2);
        QList<ServerPlayer *> listt;
        foreach (ServerPlayer *p, room->getOtherPlayers(invoke->invoker)) {
            if (p->canDiscard(invoke->invoker, "h"))
                listt << p;
        }
        if (listt.isEmpty())
            return false;
        ServerPlayer *target = room->askForPlayerChosen(invoke->invoker, listt, "fandu", "@fandu-select", false, true);
        int to_throw = room->askForCardChosen(target, invoke->invoker, "h", "fandu", false, Card::MethodDiscard);
        room->throwCard(to_throw, invoke->invoker, target);

        return false;
    }
};

#pragma message WARN("todo_fs: split ServerPlayer::pindian to 2 parts")

class Taohuan : public TriggerSkill
{
public:
    Taohuan() : TriggerSkill("taohuan")
    {
        events << CardsMoveOneTime << Pindian;
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent triggerEvent, const Room *room, const QVariant &data) const
    {
        if (triggerEvent == CardsMoveOneTime) {
            CardsMoveOneTimeStruct move = data.value<CardsMoveOneTimeStruct>();
            if (!move.from_places.contains(Player::PlaceHand) && !move.from_places.contains(Player::PlaceEquip))
                return QList<SkillInvokeDetail>();
            ServerPlayer *from = qobject_cast<ServerPlayer *>(move.from);
            if (from == NULL || from->isDead() || !from->hasSkill(this) || from->isKongcheng())
                return QList<SkillInvokeDetail>();

            // move from is now certain, we can now only check the card movement case.
            if (move.to_place == Player::DiscardPile) {
                // cae 1: discard
                if (move.reason.m_reason != CardMoveReason::S_REASON_DISMANTLE)
                    return QList<SkillInvokeDetail>();

                ServerPlayer *thrower = room->findPlayerByObjectName(move.reason.m_playerId);
                if (thrower == NULL || thrower->isDead() || thrower->isKongcheng())
                    return QList<SkillInvokeDetail>();

                QList<SkillInvokeDetail> d;
                int i = 0;
                foreach (int id, move.card_ids) {
                    const Card *c = Sanguosha->getCard(id);
                    Player::Place from_place = move.from_places.value(i++);
                    if (c != NULL && (c->isKindOf("EquipCard") || c->isKindOf("TrickCard")) && (from_place == Player::PlaceHand || from_place == Player::PlaceEquip))
                        d << SkillInvokeDetail(this, from, from, NULL, false, thrower);
                }
                return d;
            } else if (move.to_place == Player::PlaceHand || move.to_place == Player::PlaceEquip) {
                ServerPlayer *obtainer = qobject_cast<ServerPlayer *>(move.to);
                if (obtainer == NULL || obtainer->isDead() || obtainer == from || obtainer->isKongcheng())
                    return QList<SkillInvokeDetail>();

                QList<SkillInvokeDetail> d;
                for (int i = 0; i < move.card_ids.length(); ++i) {
                    Player::Place from_place = move.from_places.value(i);
                    if (from_place == Player::PlaceHand || from_place == Player::PlaceEquip)
                        d << SkillInvokeDetail(this, from, from, NULL, false, obtainer);
                }
                return d;
            }
        } else {
            PindianStruct *pindian = data.value<PindianStruct *>();
            if (pindian->isSuccess() && !pindian->to->isNude() && pindian->reason == objectName())
                return QList<SkillInvokeDetail>() << SkillInvokeDetail(this, pindian->from, pindian->from, NULL, true, pindian->to);
        }

        return QList<SkillInvokeDetail>();
    }

    bool cost(TriggerEvent triggerEvent, Room *, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const
    {
        if (triggerEvent == Pindian)
            return true;
        else {
            if (invoke->invoker->askForSkillInvoke(this, QVariant::fromValue(invoke->preferredTarget)))
                invoke->invoker->pindian(invoke->preferredTarget, objectName());
        }

        return false;
    }

    bool effect(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const
    {
        int id = room->askForCardChosen(invoke->invoker, invoke->targets.first(), "he", objectName());
        room->obtainCard(invoke->invoker, id, room->getCardPlace(id) != Player::PlaceHand);

        return false;
    }
};


class Shitu : public TriggerSkill
{
public:
    Shitu() : TriggerSkill("shitu")
    {
        events << EventPhaseChanging << DamageDone << Death << TurnStart;
    }

    void record(TriggerEvent triggerEvent, Room *room, QVariant &) const
    {
        switch (triggerEvent) {
            case DamageDone:
            case Death:
                room->setTag("shituDamageOrDeath", true);
                break;
            case TurnStart:
                room->setTag("shituDamageOrDeath", false);
                break;
            default:
                break;
        }
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent triggerEvent, const Room *room, const QVariant &data) const
    {
        if (triggerEvent == EventPhaseChanging) {
            PhaseChangeStruct change = data.value<PhaseChangeStruct>();
            if (change.player->hasSkill(this) && change.player->isAlive() && change.to == Player::NotActive) {
                QVariant extraTag = room->getTag("touhou-extra");
                QVariant damageOrDeathTag = room->getTag("shituDamageOrDeath");

                bool nowExtraTurn = extraTag.canConvert(QVariant::Bool) && extraTag.toBool();
                bool damageOrDeath = damageOrDeathTag.canConvert(QVariant::Bool) && damageOrDeathTag.toBool();
                bool extraTurnExist = room->getThread()->hasExtraTurn();

                if (!nowExtraTurn && !damageOrDeath && !extraTurnExist)
                    return QList<SkillInvokeDetail>() << SkillInvokeDetail(this, change.player, change.player);
            }
        }
        return QList<SkillInvokeDetail>();
    }

    bool cost(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const
    {
        ServerPlayer *target = room->askForPlayerChosen(invoke->invoker, room->getAllPlayers(), objectName(), "@shitu-playerchoose", true, true);
        if (target != NULL) {
            invoke->targets << target;
            return true;
        }

        return false;
    }

    bool effect(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const
    {
        room->notifySkillInvoked(invoke->invoker, objectName());
        invoke->targets.first()->setMark("shituPhase", 1);
        invoke->targets.first()->gainAnExtraTurn();

        return false;
    }
};

class Mengxian : public TriggerSkill
{
public:
    Mengxian() : TriggerSkill("mengxian")
    {
        events << EventPhaseStart;
        frequency = Wake;
    }
    
    QList<SkillInvokeDetail> triggerable(TriggerEvent, const Room *, const QVariant &data) const
    {
        ServerPlayer *merry = data.value<ServerPlayer *>();
        if (merry->hasSkill(this) && merry->getPhase() == Player::Start && merry->isAlive() && merry->getPile("jingjie").length() >= 3 && !merry->getMark("mengxian") == 0)
            return QList<SkillInvokeDetail>() << SkillInvokeDetail(this, merry, merry, NULL, true);

        return QList<SkillInvokeDetail>();
    }

    bool effect(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const
    {
        room->doLightbox("$mengxianAnimate", 4000);
        room->touhouLogmessage("#MengxianWake", invoke->invoker, objectName(), QList<ServerPlayer *>(), QString::number(invoke->invoker->getPile("jingjie").length()));
        room->notifySkillInvoked(invoke->invoker, objectName());
        room->addPlayerMark(invoke->invoker, "mengxian");
        if (room->changeMaxHpForAwakenSkill(invoke->invoker) && invoke->invoker->getMark("mengxian") > 0) {
            room->recover(invoke->invoker, RecoverStruct());
            room->handleAcquireDetachSkills(invoke->invoker, "luanying");
        }
        return false;
    }
};

class Luanying : public TriggerSkill
{
public:
    Luanying() : TriggerSkill("luanying")
    {
        events << CardUsed << CardResponded;
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent triggerEvent, const Room *room, const QVariant &data) const
    {
        ServerPlayer *user = NULL;
        const Card *card = NULL;
        if (triggerEvent == CardUsed) {
            CardUseStruct use = data.value<CardUseStruct>();
            card = use.card;
            user = use.from;
        } else if (triggerEvent == CardResponded) {
            CardResponseStruct resp = data.value<CardResponseStruct>();
            if (!resp.m_isProvision && !resp.m_isRetrial) {
                card = resp.m_card;
                user = resp.m_from;
            }
        }

        if (user == NULL || card == NULL)
            return QList<SkillInvokeDetail>();

        if (card->isKindOf("BasicCard")) {
            QList<SkillInvokeDetail> d;
            foreach (ServerPlayer *p, room->getOtherPlayers(user)) {
                if (p->hasSkill(this) && p->isAlive()) {
                    QList<int> jingjie = p->getPile("jingjie");
                    bool flag = false;
                    foreach (int id, jingjie) {
                        if (Sanguosha->getCard(id)->getColor() == card->getColor()) {
                            flag = true;
                            break;
                        }
                    }
                    if (flag)
                        d << SkillInvokeDetail(this, p, p);
                }
            }
            return d;
        }
        return QList<SkillInvokeDetail>();
    }

    bool cost(TriggerEvent triggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &data) const
    {
        ServerPlayer *user = NULL;
        const Card *card = NULL;
        if (triggerEvent == CardUsed) {
            CardUseStruct use = data.value<CardUseStruct>();
            card = use.card;
            user = use.from;
        } else if (triggerEvent == CardResponded) {
            CardResponseStruct resp = data.value<CardResponseStruct>();
            if (!resp.m_isProvision && !resp.m_isRetrial) {
                card = resp.m_card;
                user = resp.m_from;
            }
        }

        if (user == NULL || card == NULL)
            return false;

        if (invoke->invoker->askForSkillInvoke(this, data)) {
            QList<int> jingjie = invoke->invoker->getPile("jingjie");
            QList<int> availables, disables;
            foreach (int id, jingjie) {
                if (Sanguosha->getCard(id)->getColor() == card->getColor())
                    availables << id;
                else
                    disables << id;
            }

            room->fillAG(jingjie, invoke->invoker, disables);
            int selected = room->askForAG(invoke->invoker, availables, false, objectName());
            room->clearAG(invoke->invoker);


            if (selected > -1) {
                room->obtainCard(user, selected, true);
                room->touhouLogmessage("#weiya", user, objectName(), QList<ServerPlayer *>(), card->objectName());

                return true;
            }
        }

        return false;
    }

    bool effect(TriggerEvent triggerEvent, Room *, QSharedPointer<SkillInvokeDetail>, QVariant &data) const
    {
        if (triggerEvent == CardUsed) {
            CardUseStruct use = data.value<CardUseStruct>();
            use.nullified_list << "_ALL_TARGETS";
            data = QVariant::fromValue(use);
        } else if (triggerEvent == CardResponded) {
            CardResponseStruct s = data.value<CardResponseStruct>();
            s.m_isNullified = true;
            data = QVariant::fromValue(s);
        }

        return false;
    }
};

class LianxiVS : public ZeroCardViewAsSkill
{
public:
    LianxiVS() :ZeroCardViewAsSkill("lianxi")
    {
        response_pattern = "@@lianxi";
    }

    virtual const Card *viewAs() const
    {
        IronChain *ic = new IronChain(Card::NoSuit, 0);
        ic->setSkillName("_lianxi");
        return ic;
    }
};

class Lianxi : public TriggerSkill
{
public:
    Lianxi() : TriggerSkill("lianxi")
    {
        events << CardResponded << CardUsed;
        view_as_skill = new LianxiVS;
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent triggerEvent, const Room *, const QVariant &data) const
    {
        ServerPlayer *user = NULL;
        const Card *card = NULL;
        if (triggerEvent == CardUsed) {
            CardUseStruct use = data.value<CardUseStruct>();
            card = use.card;
            user = use.from;
        } else if (triggerEvent == CardResponded) {
            CardResponseStruct resp = data.value<CardResponseStruct>();
            card = resp.m_card;
            user = resp.m_from;
        }

        if (user != NULL && user->hasSkill(this) && user->isAlive() && card != NULL && card->isKindOf("Slash"))
            return QList<SkillInvokeDetail>() << SkillInvokeDetail(this, user, user);

        return QList<SkillInvokeDetail>();
    }

    bool cost(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const
    {
        room->askForUseCard(invoke->invoker, "@@lianxi", "@lianxi");
        return false;
    }
};

class Yueshi : public TriggerSkill
{
public:
    Yueshi() : TriggerSkill("yueshi")
    {
        events << EventPhaseStart;
        frequency = Wake;
    }
    
    QList<SkillInvokeDetail> triggerable(TriggerEvent, const Room *, const QVariant &data) const
    {
        ServerPlayer *toyo = data.value<ServerPlayer *>();
        if (toyo->hasSkill(this) && toyo->getPhase() == Player::Start && toyo->isAlive() && toyo->isChained() && !toyo->getMark("yueshi") == 0)
            return QList<SkillInvokeDetail>() << SkillInvokeDetail(this, toyo, toyo, NULL, true);

        return QList<SkillInvokeDetail>();
    }

    bool effect(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const
    {
        room->doLightbox("$yueshiAnimate", 4000);
        ServerPlayer *toyo = invoke->invoker;
        room->touhouLogmessage("#YueshiWake", toyo, "yueshi");
        room->notifySkillInvoked(toyo, objectName());
        room->addPlayerMark(toyo, objectName());
        if (room->changeMaxHpForAwakenSkill(toyo, 1) && toyo->getMark("yueshi") > 0)
            room->handleAcquireDetachSkills(toyo, "ruizhi");

        return false;
    }
};

#pragma message WARN("todo_Fs: develop a better method to deal with skill nullify, the skill Pingyi will be rewritten later")
// this skill is like this:
// cost: You discard one card.
// target: a skill of damage.from.
// effect:
// if you own a skill that is acquired by this method, you should lose this skill, and make the original skill become valid.
// and you make the selected skill become invalid, and you acquire this skill.

// if you or the original skill owner die, or the original skill owner lose the skill, you you should lose this skill, and make the original skill become valid.

class Pingyi : public MasochismSkill
{
public:
    Pingyi() : MasochismSkill("pingyi")
    {
    }

    QList<SkillInvokeDetail> triggerable(const Room *, const DamageStruct &damage) const
    {
        ServerPlayer *yori = damage.to;
        if (yori->isDead() || !yori->hasSkill(this) || yori->isNude())
            return QList<SkillInvokeDetail>();

        foreach (const Skill *skill, damage.from->getVisibleSkillList()) {
            if (skill->isLordSkill() || skill->isAttachedLordSkill() || skill->getFrequency() == Skill::Limited || skill->getFrequency() == Skill::Wake || skill->getFrequency() == Skill::Eternal)
                continue;

            if (!yori->hasSkill(skill, true) && damage.from->hasSkill(skill))
                return QList<SkillInvokeDetail>() << SkillInvokeDetail(this, yori, yori, NULL, false, damage.from);
        }

        return QList<SkillInvokeDetail>();
    }

    bool cost(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &data) const
    {
        ServerPlayer *yori = invoke->invoker;

        QStringList skill_names;
        
        foreach (const Skill *skill, invoke->preferredTarget->getVisibleSkillList()) {
            if (skill->isLordSkill() || skill->isAttachedLordSkill() || skill->getFrequency() == Skill::Limited || skill->getFrequency() == Skill::Wake || skill->getFrequency() == Skill::Eternal)
                continue;

            if (!yori->hasSkill(skill, true) && invoke->preferredTarget->hasSkill(skill))
                skill_names << skill->objectName();
        }

        yori->tag["pingyi_target"] = QVariant::fromValue(invoke->preferredTarget);

        if (room->askForCard(yori, ".", "@pingyi-discard:" + invoke->preferredTarget->objectName(), data, Card::MethodDiscard, NULL, false, objectName())) {
            QString skill_name = room->askForChoice(yori, objectName(), skill_names.join("+"));
            invoke->tag["selected_skill"] = skill_name;
            return true;
        }

        return false;
    }

    void onDamaged(Room *room, QSharedPointer<SkillInvokeDetail> invoke, const DamageStruct &) const
    {
        QString skill_name = invoke->tag.value("selected_skill", QString()).toString();
        if (skill_name.isEmpty())
            return;

        const Skill *skill = Sanguosha->getSkill(skill_name);
        skillProcess(room, invoke->invoker, invoke->targets.first(), skill);
    }

    static void skillProcess(Room *room, ServerPlayer *yori, ServerPlayer *skill_owner = NULL, const Skill *skill = NULL)
    {
        JsonArray arg;
        arg << (int)QSanProtocol::S_GAME_EVENT_HUASHEN;
        arg << yori->objectName();
        if (skill_owner == NULL || skill == NULL) {
            arg << QString() << QString();
        } else {
            arg << skill_owner->getGeneral()->objectName();
            arg << skill->objectName();
        }
        room->doBroadcastNotify(QSanProtocol::S_COMMAND_LOG_EVENT, arg);

        if (yori->tag.contains("pingyi_skill") && yori->tag.contains("pingyi_originalOwner")) {
            QString originalSkillName = yori->tag.value("pingyi_skill", QString()).toString();
            ServerPlayer *originalSkillOwner = yori->tag.value("pingyi_originalOwner").value<ServerPlayer *>();
            yori->tag.remove("pingyi_skill");
            yori->tag.remove("pingyi_originalOwner");

            // 1. yorihime lost the acquired skill
            if (!originalSkillName.isEmpty())
                room->handleAcquireDetachSkills(yori, "-" + originalSkillName, true);

            // 2. let the original skill become valid
            if (originalSkillOwner != NULL) {
                skill_owner->tag.remove("pingyi_from");
                skill_owner->tag.remove("pingyi_invalidSkill");
                room->setPlayerSkillInvalidity(originalSkillOwner, originalSkillName, false);
            }
        }

        if (skill != NULL && skill_owner != NULL) {
            yori->tag["pingyi_skill"] = skill->objectName();
            yori->tag["pingyi_originalOwner"] = QVariant::fromValue(skill_owner);
            skill_owner->tag["pingyi_from"] = QVariant::fromValue(yori);
            skill_owner->tag["pingyi_invalidSkill"] = skill->objectName();

            // 3.let the skill become invalid
            room->setPlayerSkillInvalidity(skill_owner, skill, true);

            // 4. acquire the skill
            room->handleAcquireDetachSkills(yori, skill->objectName(), true);
        }
    }
};


class PingyiHandler : public TriggerSkill
{
public:
    PingyiHandler() : TriggerSkill("#pingyi_handle")
    {
        events << EventLoseSkill << Death;
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent triggerEvent, const Room *, const QVariant &data) const
    {
        if (triggerEvent == EventLoseSkill) {
            SkillAcquireDetachStruct ad = data.value<SkillAcquireDetachStruct>();
            if (ad.isAcquire)
                return QList<SkillInvokeDetail>();

            if (ad.skill->objectName() == "pingyi")
                return QList<SkillInvokeDetail>() << SkillInvokeDetail(this, ad.player, ad.player, NULL, true);
            else {
                if (ad.player->tag.contains("pingyi_invalidSkill") && ad.player->tag.value("pingyi_invalidSkill").toString() == ad.skill->objectName()) {
                    ServerPlayer *yori = ad.player->tag.value("pingyi_from").value<ServerPlayer *>();
                    return QList<SkillInvokeDetail>() << SkillInvokeDetail(this, yori, yori, NULL, true);
                }
            }
        } else {
            DeathStruct death = data.value<DeathStruct>();
            if (death.who->tag.contains("pingyi_invalidSkill")) {
                ServerPlayer *yori = death.who->tag.value("pingyi_from").value<ServerPlayer *>();
                return QList<SkillInvokeDetail>() << SkillInvokeDetail(this, yori, yori, NULL, true);
            }
        }

        return QList<SkillInvokeDetail>();
    }

    bool effect(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const
    {
        Pingyi::skillProcess(room, invoke->invoker);
        return false;
    }
};

ZhesheCard::ZhesheCard()
{
    will_throw = true;
}

bool ZhesheCard::targetFilter(const QList<const Player *> &targets, const Player *, const Player *) const
{
    return targets.length() == 0;
}

void ZhesheCard::onEffect(const CardEffectStruct &effect) const
{
    effect.from->tag["zheshe_target"] = QVariant::fromValue(effect.to);
}


class ZhesheVS : public OneCardViewAsSkill
{
public:
    ZhesheVS() :OneCardViewAsSkill("zheshe")
    {
        filter_pattern = ".|.|.|hand!";//"^EquipCard!" ;
        response_pattern = "@@zheshe";
    }


    virtual const Card *viewAs(const Card *originalCard) const
    {
        ZhesheCard *card = new ZhesheCard;
        card->addSubcard(originalCard);

        return card;
    }
};

class Zheshe : public TriggerSkill
{
public:
    Zheshe() : TriggerSkill("zheshe")
    {
        events << DamageInflicted << DamageComplete;
        view_as_skill = new ZhesheVS;
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent triggerEvent, const Room *, const QVariant &data) const
    {
        DamageStruct damage = data.value<DamageStruct>();
        if (triggerEvent == DamageInflicted) {
            if (damage.to->hasSkill(this) && damage.to->isAlive() && !damage.to->isKongcheng() && damage.to != damage.from)
                return QList<SkillInvokeDetail>() << SkillInvokeDetail(this, damage.to, damage.to);
        } else if (triggerEvent == DamageComplete) {
            if (damage.to->isAlive() && damage.to->getMark("zheshetransfer") > 0 && damage.reason == "zheshe")
                return QList<SkillInvokeDetail>() << SkillInvokeDetail(this, damage.from, damage.from, NULL, true, damage.to);
        }

        return QList<SkillInvokeDetail>();
    }


    bool cost(TriggerEvent triggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const
    {
        if (triggerEvent == DamageInflicted) {
            if (room->askForUseCard(invoke->invoker, "@@zheshe", "@zheshe", -1, Card::MethodDiscard) && invoke->invoker->tag.contains("zheshe_target")) {
                ServerPlayer *target = invoke->invoker->tag.value("zheshe_target", QVariant::fromValue((ServerPlayer *)NULL)).value<ServerPlayer *>();
                if (target != NULL) {
                    invoke->targets << target;
                    return true;
                }
            }
        } else if (triggerEvent == DamageComplete) {
            invoke->preferredTarget->setMark("zheshetransfer", 0);
            return true;
        }

        return false;
    }

    bool effect(TriggerEvent triggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const
    {
        if (triggerEvent == DamageInflicted) {
            invoke->targets.first()->setMark("zheshetransfer", 1);

            DamageStruct damage;
            damage.from = invoke->invoker;
            damage.to = invoke->targets.first();
            damage.reason = "zheshe";
            room->damage(damage);
            return true;
        } else if (triggerEvent == DamageComplete)
            invoke->targets.first()->drawCards(invoke->targets.first()->getLostHp(), objectName());

        return false;
    }
};


class Tanchi : public TriggerSkill
{
public:
    Tanchi() : TriggerSkill("tanchi")
    {
        events << PreHpRecover;
        frequency = Compulsory;
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent, const Room *, const QVariant &data) const
    {
        RecoverStruct recover = data.value<RecoverStruct>();
        if (recover.to->hasSkill(this))
            return QList<SkillInvokeDetail>() << SkillInvokeDetail(this, recover.to, recover.to, NULL, true);

        return QList<SkillInvokeDetail>();
    }

    bool effect(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &data) const
    {
        room->touhouLogmessage("#TriggerSkill", invoke->invoker, objectName());
        room->notifySkillInvoked(invoke->invoker, objectName());
        RecoverStruct recover = data.value<RecoverStruct>();
        recover.recover = recover.recover + 1;
        data = QVariant::fromValue(recover);
        return false;
    }
};

ZhuonongCard::ZhuonongCard()
{
    m_skillName = "zhuonong";
}

bool ZhuonongCard::targetFilter(const QList<const Player *> &targets, const Player *, const Player *) const
{
    return targets.length() == 0;
}

void ZhuonongCard::onEffect(const CardEffectStruct &effect) const
{
    Room *room = effect.to->getRoom();
    effect.from->tag["zhuonong_target"] = QVariant::fromValue(effect.to);
    QString choice = room->askForChoice(effect.from, "zhuonong", "rd+dr", QVariant::fromValue(effect.to));
    RecoverStruct recover;
    recover.who = effect.from;
    DamageStruct damage(objectName(), effect.from, effect.to, 1, DamageStruct::Fire);
    if (choice == "rd" && effect.to->isWounded())
        room->recover(effect.to, recover);
    room->damage(damage);
    if (choice == "dr" && effect.to->isWounded() && effect.to->isAlive())
        room->recover(effect.to, recover);
}

class Zhuonong : public ZeroCardViewAsSkill
{
public:
    Zhuonong() :ZeroCardViewAsSkill("zhuonong")
    {
    }

    virtual bool isEnabledAtPlay(const Player *player) const
    {
        return !player->hasUsed("ZhuonongCard");
    }

    virtual const Card *viewAs() const
    {
        return new ZhuonongCard;
    }
};

class Jijing : public TriggerSkill
{
public:
    Jijing() : TriggerSkill("jijing")
    {
        events << Damaged;
        frequency = Compulsory;
    }
    
    QList<SkillInvokeDetail> triggerable(TriggerEvent, const Room *room, const QVariant &data) const
    {
        DamageStruct damage = data.value<DamageStruct>();
        if (damage.to->isDead())
            return QList<SkillInvokeDetail>();

        ServerPlayer *p = room->getCurrent();
        if (!p->isCurrent())
            return QList<SkillInvokeDetail>();

        if (p->hasSkill(this) && damage.to->getMark("@jijing") == 0 && damage.to != p)
            return QList<SkillInvokeDetail>() << SkillInvokeDetail(this, p, p, NULL, true, damage.to);

        return QList<SkillInvokeDetail>();
    }
    
    bool effect(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const
    {
        room->notifySkillInvoked(invoke->invoker, objectName());
        room->setFixedDistance(invoke->invoker, invoke->targets.first(), 1);
        invoke->targets.first()->gainMark("@jijing");
        room->setPlayerFlag(invoke->invoker, "jijing");
        return false;
    }
};

class JijingClear : public TriggerSkill
{
public:
    JijingClear() : TriggerSkill("#jijing")
    {
        events << EventPhaseChanging;
    }
    
    QList<SkillInvokeDetail> triggerable(TriggerEvent, const Room *room, const QVariant &data) const
    {
        PhaseChangeStruct change = data.value<PhaseChangeStruct>();
        if (change.to == Player::NotActive && change.player->hasFlag("jijing")) {
            foreach (ServerPlayer *p, room->getOtherPlayers(change.player)) {
                if (p->getMark("@jijing") > 0)
                    return QList<SkillInvokeDetail>() << SkillInvokeDetail(this, change.player, change.player, NULL, true);
            }
        }

        return QList<SkillInvokeDetail>();
    }

    bool cost(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const
    {
        foreach (ServerPlayer *p, room->getOtherPlayers(invoke->invoker)) {
            if (p->getMark("@jijing") > 0)
                invoke->targets << p;
        }
        return true;
    }
    
    bool effect(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const
    {
        foreach (ServerPlayer *p, invoke->targets) {
            p->loseAllMarks("@jijing");
            room->setFixedDistance(invoke->invoker, p, -1);
        }

        return false;
    }
};


class Ganying;
namespace
{
    Ganying *ganying_instance;
}

class Ganying : public TriggerSkill
{
public:
    Ganying() : TriggerSkill("ganying")
    {
        ganying_instance = this;
    }

    bool effect(TriggerEvent triggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const
    {
        invoke->invoker->drawCards(1, objectName());
        QStringList distance_changed = invoke->invoker->tag.value("distance_changed_" + QString::number(triggerEvent), QStringList()).toStringList();
        if (distance_changed.isEmpty())
            return false;

        QList<ServerPlayer *> targets;
        foreach (const QString &c, distance_changed) {
            ServerPlayer *p = room->findPlayerByObjectName(c);
            if (p != NULL)
                targets << p;
        }

        if (targets.isEmpty())
            return false;

        ServerPlayer *target = room->askForPlayerChosen(invoke->invoker, targets, "ganying", "@ganying", true);
        if (target == NULL)
            return false;

        int id = room->askForCardChosen(invoke->invoker, target, "h", "ganying", false, Card::MethodDiscard);
        room->throwCard(id, target, invoke->invoker == target ? NULL : invoke->invoker);

        return false;
    }
};

class GanyingHandler : public TriggerSkill
{
public:
    GanyingHandler() : TriggerSkill("ganying_handle")
    {
        events << NumOfEvents;
        global = true;
    }

    void record(TriggerEvent triggerEvent, Room *room, QVariant &) const
    {
        if (triggerEvent == GameStart) {
            foreach (ServerPlayer *p, room->getAllPlayers()) {
                QVariantMap distance_map;
                foreach (ServerPlayer *q, room->getOtherPlayers(p))
                    distance_map[q->objectName()] = p->distanceTo(q);

                p->tag["distance_map"] = distance_map;
            }
            return;
        }

        foreach (ServerPlayer *p, room->getAllPlayers()) {
            p->tag.remove("distance_changed_" + QString::number(triggerEvent));
            QVariantMap distance_map;
            foreach (ServerPlayer *q, room->getOtherPlayers(p))
                distance_map[q->objectName()] = p->distanceTo(q);

            QStringList distance_changed;
            QVariantMap distance_map_old = p->tag.value("distance_map", QVariantMap()).toMap();

            foreach (const QString &key, distance_map_old.keys()) {
                bool ok = false;
                if (distance_map.value(key).toInt(&ok) != -1 && ok) {
                    int old_distance = distance_map_old.value(key).toInt();
                    int new_distance = distance_map.value(key).toInt();
                    if (old_distance != new_distance)
                        distance_changed << key;
                } else
                    distance_map[key] = -1;
            }

            p->tag["distance_changed_" + QString::number(triggerEvent)] = distance_changed;
            p->tag["distance_map"] = distance_map;
        }
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent triggerEvent, const Room *room, const QVariant &) const
    {
        QList<SkillInvokeDetail> d;
        foreach (ServerPlayer *p, room->getAllPlayers()) {
            if (!p->hasSkill("ganying"))
                continue;

            QStringList distance_changed = p->tag.value("distance_changed_" + QString::number(triggerEvent), QStringList()).toStringList();
            if (!distance_changed.isEmpty())
                d << SkillInvokeDetail(ganying_instance, p, p);
        }
        return d;
    }
};

class Zhujiu : public TriggerSkill
{
public:
    Zhujiu() : TriggerSkill("zhujiu")
    {
        events << TargetConfirmed;
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent, const Room *room, const QVariant &data) const
    {
        CardUseStruct use = data.value<CardUseStruct>();
        if (use.card->isKindOf("Peach") || use.card->isKindOf("Analeptic")) {
            QList<SkillInvokeDetail> d;
            foreach (ServerPlayer *to, use.to) {
                foreach (ServerPlayer *p, room->getOtherPlayers(to)) {
                    if (p->hasSkill(this))
                        d << SkillInvokeDetail(this, p, to);
                }
            }
            return d;
        }

        return QList<SkillInvokeDetail>();
    }

    bool cost(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &data) const
    {
        const Card *c = room->askForCard(invoke->owner, "..", "@zhujiu:" + invoke->invoker->objectName(), data, Card::MethodNone);
        if (c) {
            CardMoveReason r(CardMoveReason::S_REASON_GIVE, invoke->owner->objectName(), objectName(), QString());
            room->obtainCard(invoke->invoker, c, r, room->getCardPlace(c->getEffectiveId()) != Player::PlaceHand);

            return true;
        }

        return false;
    }

    bool effect(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const
    {
        room->notifySkillInvoked(invoke->owner, objectName());
        invoke->owner->drawCards(1, objectName());

        return false;
    }
};


YushouCard::YushouCard()
{
    m_skillName = "yushou";
    mute = true;
}

bool YushouCard::targetFilter(const QList<const Player *> &targets, const Player *to_select, const Player *) const
{

    if (targets.length() == 0) {
        return !to_select->getEquips().isEmpty();
    } else if (targets.length() == 1) {
        foreach (const Card *e, targets.first()->getEquips()) {
            const EquipCard *equip = qobject_cast<const EquipCard *>(e->getRealCard());
            if (!to_select->getEquip(equip->location()))
                return true;
        }
    }
    return false;
}

bool YushouCard::targetsFeasible(const QList<const Player *> &targets, const Player *) const
{
    return targets.length() == 2;
}

void YushouCard::onUse(Room *room, const CardUseStruct &card_use) const
{
    ServerPlayer *from = card_use.from;
    ServerPlayer *to1 = card_use.to.at(0);
    ServerPlayer *to2 = card_use.to.at(1);
    QList<ServerPlayer *>logto;
    logto << to1 << to2;
    room->touhouLogmessage("#ChoosePlayerWithSkill", from, "yushou", logto, "");
    room->notifySkillInvoked(card_use.from, "yushou");
    QList<int> disable;


    foreach (const Card *e, to1->getEquips()) {
        const EquipCard *equip = qobject_cast<const EquipCard *>(e->getRealCard());
        if (to2->getEquip(equip->location())) {
            disable << e->getEffectiveId();
        }
    }
    from->tag["yushou_target"] = QVariant::fromValue(to2);
    to1->tag["yushou_target"] = QVariant::fromValue(to2);
    int card_id = room->askForCardChosen(from, to1, "e", "yushou", false, Card::MethodNone, disable);

    const Card *card = Sanguosha->getCard(card_id);
    room->moveCardTo(card, to1, to2, Player::PlaceEquip,
        CardMoveReason(CardMoveReason::S_REASON_TRANSFER,
        from->objectName(), "yushou", QString()));

    QString choice = room->askForChoice(to1, "yushou", "damage+cancel");
    if (choice == "damage")
        room->damage(DamageStruct("yushou", to1, to2, 1, DamageStruct::Normal));

}

class Yushou : public ZeroCardViewAsSkill
{
public:
    Yushou() :ZeroCardViewAsSkill("yushou")
    {
    }

    virtual bool isEnabledAtPlay(const Player *player) const
    {
        return !player->hasUsed("YushouCard");
    }

    virtual const Card *viewAs() const
    {
        return new YushouCard;
    }
};

PanduCard::PanduCard()
{
    handling_method = Card::MethodUse;
    mute = true;
}

bool PanduCard::targetFilter(const QList<const Player *> &targets, const Player *to_select, const Player *Self) const
{
    return targets.length() == 0 && !to_select->isKongcheng() && to_select != Self;
}

void PanduCard::use(Room *room, ServerPlayer *source, QList<ServerPlayer *> &targets) const
{
    ServerPlayer *target = targets.first();
    int card_id = room->askForCardChosen(source, target, "h", "pandu");
    room->showCard(target, card_id);
    Card *showcard = Sanguosha->getCard(card_id);
    if (showcard->isKindOf("Slash")) {
        if (!target->isCardLimited(showcard, Card::MethodUse)) {
            room->setCardFlag(showcard, "pandu");
            room->useCard(CardUseStruct(showcard, target, source), false);
        }
    } else if (!showcard->isKindOf("BasicCard"))
        room->obtainCard(source, showcard, true);

}

class Pandu : public ZeroCardViewAsSkill
{
public:
    Pandu() : ZeroCardViewAsSkill("pandu")
    {
    }

    virtual bool isEnabledAtPlay(const Player *player) const
    {
        return !player->hasUsed("PanduCard");
    }

    virtual const Card *viewAs() const
    {
        return new PanduCard;
    }
};

class Bihuo : public TriggerSkill
{
public:
    Bihuo() : TriggerSkill("bihuo")
    {
        events << TargetConfirming;
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent, const Room *room, const QVariant &data) const
    {
        CardUseStruct use = data.value<CardUseStruct>();
        QList<SkillInvokeDetail> d;
        foreach (ServerPlayer *p, use.to) {
            if (!p->hasSkill(this) || p->isKongcheng())
                continue;

            bool flag = false;
            foreach (ServerPlayer *q, room->getOtherPlayers(p)) {
                if (!(use.from == q || use.to.contains(q)) && use.from->canSlash(q, use.card, false)) {
                    flag = true;
                    break;
                }
            }

            if (flag)
                d << SkillInvokeDetail(this, p, p);
        }

        return d;
    }

    bool cost(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &data) const
    {
        CardUseStruct use = data.value<CardUseStruct>();
        QList<ServerPlayer *> targets;
        foreach (ServerPlayer *q, room->getOtherPlayers(invoke->invoker)) {
            if (!(use.from == q || use.to.contains(q)) && use.from->canSlash(q, use.card, false))
                targets << q;
        }

        QString prompt = "@bihuo-playerchosen:" + use.from->objectName();
        ServerPlayer *target = room->askForPlayerChosen(invoke->invoker, targets, objectName(), prompt, true, true);
        if (target) {
            QVariantMap bihuo_list = target->tag.value("bihuo", QVariantMap()).toMap();
            int i = bihuo_list.value(invoke->invoker->objectName(), 0).toInt();
            i += invoke->invoker->getHandcardNum();
            bihuo_list[invoke->invoker->objectName()] = i;
            target->tag["bihuo"] = bihuo_list;

            const Card *c = invoke->invoker->wholeHandCards();
            CardMoveReason r(CardMoveReason::S_REASON_GIVE, invoke->owner->objectName(), objectName(), QString());
            room->obtainCard(target, c, r, false);
            delete c;
            
            invoke->targets << target;
            return true;
        }

        return false;
    }

    bool effect(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &data) const
    {
        CardUseStruct use = data.value<CardUseStruct>();
        use.to << invoke->targets;
        use.to.removeOne(invoke->invoker);
        QList<ServerPlayer *> logto;
        logto << invoke->invoker;
        QList<ServerPlayer *> logto1;
        logto1 << invoke->targets;
        room->touhouLogmessage("$CancelTarget", use.from, use.card->objectName(), logto);
        room->touhouLogmessage("#huzhu_change", use.from, use.card->objectName(), logto1);

        return false;
    }
};

class BihuoReturn : public TriggerSkill
{
public:
    BihuoReturn() : TriggerSkill("#bihuo")
    {
        events << EventPhaseChanging;
    }
    
    QList<SkillInvokeDetail> triggerable(TriggerEvent, const Room *room, const QVariant &data) const
    {
        PhaseChangeStruct change = data.value<PhaseChangeStruct>();
        if (change.to == Player::NotActive) {
            QList<SkillInvokeDetail> d;
            foreach (ServerPlayer *p, room->getAllPlayers()) {
                QVariantMap bihuo_list = p->tag.value("bihuo", QVariantMap()).toMap();
                p->tag.remove("bihuo");
                foreach (const QString &key, bihuo_list.keys()) {
                    int n = bihuo_list.value(key, 0).toInt();
                    if (n <= 0)
                        continue;

                    ServerPlayer *kos = room->findPlayerByObjectName(key);
                    if (kos == NULL || kos->isDead())
                        continue;

                    SkillInvokeDetail s(this, kos, p, NULL, true);
                    s.tag["n"] = n;
                    d << s;
                }
            }
            return d;
        }

        return QList<SkillInvokeDetail>();
    }
    

    bool effect(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const
    {
        int n = invoke->tag.value("n", 0).toInt();
        if (n <= 0)
            return false;

        const Card *c = room->askForExchange(invoke->invoker, objectName(), n, n, false, "@bihuo-return");
        CardMoveReason r(CardMoveReason::S_REASON_GIVE, invoke->owner->objectName(), objectName(), QString());
        room->obtainCard(invoke->owner, c, r, false);
        delete c;
        return false;
    }
};



TH99Package::TH99Package()
    : Package("th99")
{
    General *akyuu = new General(this, "akyuu$", "wai", 3, false);
    akyuu->addSkill(new Qiuwen);
    akyuu->addSkill(new Zaozu);
    akyuu->addSkill(new Dangjia);

    General *rinnosuke = new General(this, "rinnosuke", "wai");
    rinnosuke->addSkill(new XiufuDebug);

    General *tokiko = new General(this, "tokiko", "wai", 3, false);
    tokiko->addSkill(new Fandu);
    tokiko->addSkill(new Taohuan);

    General *renko = new General(this, "renko", "wai", 4, false);
    renko->addSkill(new Shitu);

    General *merry = new General(this, "merry", "wai", 4, false);
    merry->addSkill("jingjie");
    merry->addSkill(new Mengxian);
    merry->addRelateSkill("luanying");

    General *toyohime = new General(this, "toyohime", "wai", 3, false);
    toyohime->addSkill(new Lianxi);
    toyohime->addSkill(new Yueshi);

    General *yorihime = new General(this, "yorihime", "wai", 4, false);
    yorihime->addSkill(new Pingyi);
    yorihime->addSkill(new PingyiHandler);
    related_skills.insertMulti("pingyi", "#pingyi_handle");


    General *sunny = new General(this, "sunny", "wai", 4, false);
    sunny->addSkill(new Zheshe);
    sunny->addSkill(new Tanchi);

    General *lunar = new General(this, "lunar", "wai", 4, false);
    lunar->addSkill(new Zhuonong);
    lunar->addSkill(new Jijing);
    lunar->addSkill(new JijingClear);
    related_skills.insertMulti("jijing", "#jijing");

    General *star = new General(this, "star", "wai", 4, false);
    star->addSkill(new Ganying);

    General *kasen = new General(this, "kasen", "wai", 4, false);
    kasen->addSkill(new Zhujiu);
    kasen->addSkill(new Yushou);

    General *kosuzu = new General(this, "kosuzu", "wai", 3, false);
    kosuzu->addSkill(new Pandu);
    kosuzu->addSkill(new Bihuo);
    kosuzu->addSkill(new BihuoReturn);
    related_skills.insertMulti("bihuo", "#bihuo");

    General *mamizou = new General(this, "mamizou_sp", "wai", 4, false);
    Q_UNUSED(mamizou);

    General *reisen2 = new General(this, "reisen2", "wai", 4, false);
    Q_UNUSED(reisen2);

    addMetaObject<QiuwenCard>();
    addMetaObject<DangjiaCard>();
    addMetaObject<XiufuCard>();
    addMetaObject<XiufuMoveCard>();
    addMetaObject<ZhesheCard>();
    addMetaObject<ZhuonongCard>();
    addMetaObject<YushouCard>();
    addMetaObject<PanduCard>();
    skills << new DangjiaVS << new Luanying << new GanyingHandler << new XiufuMove;
}

ADD_PACKAGE(TH99)

