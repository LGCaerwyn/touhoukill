#include "th07.h"

#include "general.h"
#include "skill.h"
#include "engine.h"
#include "standard.h"
#include "client.h"





class Sidie : public TriggerSkill
{
public:
    Sidie() : TriggerSkill("sidie")
    {
        events << DamageCaused << EventPhaseChanging;
    }

    void record(TriggerEvent triggerEvent, Room *room, QVariant &) const
    {
        if (triggerEvent == EventPhaseChanging) {
            foreach (ServerPlayer *p, room->getAllPlayers()) {
                if (p->hasFlag("sidie_used"))
                    room->setPlayerFlag(p, "-sidie_used");
            }
        }
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent triggerEvent, const Room *room, const QVariant &data) const
    {
        if (triggerEvent == DamageCaused) {
            DamageStruct damage = data.value<DamageStruct>();
            if (damage.chain || damage.transfer || !damage.by_user)
                return  QList<SkillInvokeDetail>();
            if (!damage.from || !damage.to || damage.from == damage.to)
                return  QList<SkillInvokeDetail>();

            if (damage.from->hasFlag("sidie_used") || !damage.from->hasSkill(this))
                return  QList<SkillInvokeDetail>();
            if (damage.card && damage.card->isKindOf("Slash")) {
                Slash *slash = new Slash(Card::NoSuit, 0);
                slash->deleteLater();
                if (damage.to->isCardLimited(slash, Card::MethodUse))
                    return  QList<SkillInvokeDetail>();
                foreach (ServerPlayer *p, room->getAllPlayers()) {
                    if (damage.to->canSlash(p, slash, false))
                        return  QList<SkillInvokeDetail>() << SkillInvokeDetail(this, damage.from, damage.from);
                }
            }
        }
        return  QList<SkillInvokeDetail>();
    }

    bool cost(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &data) const
    {
        DamageStruct damage = data.value<DamageStruct>();
        ServerPlayer *yuyuko = invoke->invoker;
        QList<ServerPlayer *> listt;
        foreach (ServerPlayer *p, room->getAlivePlayers()) {
            if (damage.to->canSlash(p, NULL, false))
                listt << p;
        }
        yuyuko->tag["sidie_target"] = QVariant::fromValue(damage.to);
        ServerPlayer *target = room->askForPlayerChosen(yuyuko, listt, objectName(), "@sidie:" + damage.to->objectName(), true, true);
        yuyuko->tag.remove("sidie_target");
        if (target) {
            invoke->targets << target;
            ServerPlayer *current = room->getCurrent();
            if (current && current->isAlive())
                room->setPlayerFlag(yuyuko, "sidie_used");
            QList<ServerPlayer *> logto;
            logto << damage.to;
            room->touhouLogmessage("#Dongjie", yuyuko, "sidie", logto);
            return true;
        }
        return false;
    }

    bool effect(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &data) const
    {
        DamageStruct damage = data.value<DamageStruct>();
        ServerPlayer *yuyuko = invoke->invoker;
        Slash *slash = new Slash(Card::NoSuit, 0);
        slash->deleteLater();
        ServerPlayer *target = invoke->targets.first();

        if (target) {
            yuyuko->drawCards(2);
            slash->setSkillName("_" + objectName());
            room->useCard(CardUseStruct(slash, damage.to, target), false);
            return true;
        }

        return false;
    }
};

class Wangxiang : public TriggerSkill
{
public:
    Wangxiang() : TriggerSkill("wangxiang$")
    {
        events << Damaged << FinishJudge;
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent event, const Room *room, const QVariant &data) const
    {
        QList<SkillInvokeDetail> details;
        if (event == Damaged) {
            DamageStruct damage = data.value<DamageStruct>();
            if (damage.to->isAlive() && damage.to->getKingdom() == "yym") {
                foreach (ServerPlayer *p, room->getOtherPlayers(damage.to)) {
                    if (p->hasLordSkill("wangxiang"))
                        details << SkillInvokeDetail(this, p, damage.to);
                }
            }
        } else if (event == FinishJudge) {
            JudgeStruct * judge = data.value<JudgeStruct *>();
            if (judge->reason == objectName() && judge->isGood()) {
                ServerPlayer *uuz = judge->who->tag["uuz_wangxiang"].value<ServerPlayer *>();
                if (uuz)
                    details << SkillInvokeDetail(this, uuz, judge->who, NULL, true);
            }
        }
        return details;
    }


    bool cost(TriggerEvent triggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const
    {
        if (triggerEvent == Damaged) {
            invoke->invoker->tag["wangxiang-target"] = QVariant::fromValue(invoke->owner);
            bool doJudge = invoke->invoker->askForSkillInvoke(objectName(), QVariant::fromValue(invoke->owner));
            invoke->invoker->tag.remove("wangxiang-target");
            if (doJudge) {
                room->broadcastSkillInvoke(objectName());
                room->notifySkillInvoked(invoke->owner, objectName());
                LogMessage log;
                log.type = "#InvokeOthersSkill";
                log.from = invoke->invoker;
                log.to << invoke->owner;
                log.arg = objectName();
                room->sendLog(log);
                return true;
            }
        } else {
            invoke->invoker->tag.remove("uuz_wangxiang");
            return true;
        }
        return false;
    }

    bool effect(TriggerEvent triggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &data) const
    {
        if (triggerEvent == Damaged) {
            invoke->invoker->tag["uuz_wangxiang"] = QVariant::fromValue(invoke->owner);

            JudgeStruct judge;
            judge.pattern = ".|black";
            judge.who = invoke->invoker;
            judge.reason = objectName();
            judge.good = true;
            room->judge(judge);
        } else {
            JudgeStruct * judge = data.value<JudgeStruct *>();
            invoke->owner->obtainCard(judge->card);
        }
        return false;
    }
};

class Jingjie : public TriggerSkill
{
public:
    Jingjie() : TriggerSkill("jingjie")
    {
        frequency = Frequent;
        events << Damaged << AfterDrawNCards;
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent triggerEvent, const Room *room, const QVariant &data) const
    {
        QList<SkillInvokeDetail> d;
        ServerPlayer *yukari = NULL;
        int num = 1;
        if (triggerEvent == AfterDrawNCards) {
            yukari = room->getCurrent();
        } else if (triggerEvent == Damaged) {
            num = data.value<DamageStruct>().damage;
            yukari = data.value<DamageStruct>().to;
        }

        QStringList trigger_list;
        if (yukari && yukari->hasSkill(this)) {
            for (int i = 1; i <= num; i++) {
                d << SkillInvokeDetail(this, yukari, yukari);
            }
        }
        return d;
    }

    //default cost

    bool effect(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const
    {
        invoke->invoker->drawCards(1);
        if (!invoke->invoker->isKongcheng()) {
            const Card *cards = room->askForExchange(invoke->invoker, objectName(), 1, 1, false, "jingjie_exchange");
            DELETE_OVER_SCOPE(const Card, cards)
            int id = cards->getSubcards().first();
            invoke->invoker->addToPile("jingjie", id);
        }
        return false;
    }

};


class Sisheng : public TriggerSkill
{
public:
    Sisheng() : TriggerSkill("sisheng")
    {
        events << Dying;
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent, const Room *room, const QVariant &data) const
    {
        QList<SkillInvokeDetail> d;
        ServerPlayer *who = data.value<DyingStruct>().who;
        QList<ServerPlayer *> yukaris = room->findPlayersBySkillName(objectName());
        foreach (ServerPlayer *p, yukaris) {
            if (who && p->getPile("jingjie").length() > 0 && p->canDiscard(who, "he") && who->getHp() < 1) {
                d << SkillInvokeDetail(this, p, p);
            }
        }
        return d;
    }

    bool cost(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &data) const
    {
        ServerPlayer *yukari = invoke->invoker;
        if (yukari->askForSkillInvoke(objectName(), data)) {
            QList<int> ids = yukari->getPile("jingjie");
            room->fillAG(ids, yukari);
            int id = room->askForAG(yukari, ids, false, objectName());
            CardMoveReason reason(CardMoveReason::S_REASON_REMOVE_FROM_PILE, "", NULL, objectName(), "");
            room->clearAG(yukari);
            room->throwCard(Sanguosha->getCard(id), reason, NULL);
            return true;
        }
        return false;
    }

    bool effect(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &data) const
    {
        ServerPlayer *yukari = invoke->invoker;
        ServerPlayer *who = data.value<DyingStruct>().who;
        room->doAnimate(QSanProtocol::S_ANIMATE_INDICATE, yukari->objectName(), who->objectName());
        int id2 = room->askForCardChosen(yukari, who, "he", objectName(), false, Card::MethodDiscard);
        if (yukari->canDiscard(who, id2))
            room->throwCard(id2, who, yukari);

        RecoverStruct recover;
        recover.recover = 1;
        recover.who = yukari;
        room->recover(who, recover);
        return false;
    }
};

class Jingdong : public TriggerSkill
{
public:
    Jingdong() : TriggerSkill("jingdong")
    {
        events << EventPhaseChanging;
    }


    QList<SkillInvokeDetail> triggerable(TriggerEvent, const Room *room, const QVariant &data) const
    {
        QList<SkillInvokeDetail> d;
        PhaseChangeStruct change = data.value<PhaseChangeStruct>();
        if (change.to == Player::Discard) {
            QList<ServerPlayer *> yukaris = room->findPlayersBySkillName(objectName());
            foreach (ServerPlayer *yukari, yukaris) {
                if (!yukari->getPile("jingjie").isEmpty())
                    d << SkillInvokeDetail(this, yukari, yukari);
            }
        }
        return d;
    }

    bool cost(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &data) const
    {
        ServerPlayer *yukari = invoke->invoker;
        ServerPlayer *player = data.value<PhaseChangeStruct>().player;
        yukari->tag["jingdong_target"] = QVariant::fromValue(player);
        QString prompt = "target:" + player->objectName();
        bool skip = room->askForSkillInvoke(yukari, objectName(), prompt);
        yukari->tag.remove("jingdong_target");
        if (skip) {
            QList<int> pile = yukari->getPile("jingjie");
            room->fillAG(pile, yukari);
            int id = room->askForAG(yukari, pile, false, objectName());
            CardMoveReason reason(CardMoveReason::S_REASON_REMOVE_FROM_PILE, "", NULL, objectName(), "");
            room->throwCard(Sanguosha->getCard(id), reason, NULL);
            room->clearAG(yukari);
            room->doAnimate(QSanProtocol::S_ANIMATE_INDICATE, yukari->objectName(), player->objectName());
            return true;
        }
        return false;
    }

    bool effect(TriggerEvent, Room *, QSharedPointer<SkillInvokeDetail>, QVariant &data) const
    {
        ServerPlayer *player = data.value<PhaseChangeStruct>().player;
        player->skip(Player::Discard);
        return false;
    }
};

class ZhaoliaoVS : public OneCardViewAsSkill
{
public:
    ZhaoliaoVS() : OneCardViewAsSkill("zhaoliaoVS")
    {
        response_pattern = "@@zhaoliao!";
        response_or_use = true;
    }

    virtual bool viewFilter(const Card *c) const
    {
        return !c->isEquipped();
    }

    virtual const Card *viewAs(const Card *c) const
    {
        ExNihilo *exnihilo = new ExNihilo(Card::SuitToBeDecided, 0);
        exnihilo->addSubcard(c);
        exnihilo->setSkillName("_zhaoliao");
        return exnihilo;
    }
};

class Zhaoliao : public TriggerSkill
{
public:
    Zhaoliao() : TriggerSkill("zhaoliao")
    {
        events << Damaged;
        view_as_skill = new ZhaoliaoVS;
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent, const Room *room, const QVariant &data) const
    {
        QList<SkillInvokeDetail> d;
        ServerPlayer *a = data.value<DamageStruct>().to;
        QList<ServerPlayer *> rans = room->findPlayersBySkillName(objectName());
        foreach (ServerPlayer *ran, rans) {
            if (a->isAlive() && a != ran && !ran->isNude())
                d << SkillInvokeDetail(this, ran, ran);
        }
        return d;
    }

    bool cost(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &data) const
    {
        ServerPlayer *a = data.value<DamageStruct>().to;
        ServerPlayer *ran = invoke->invoker;
        ran->tag["zhaoliao_target"] = QVariant::fromValue(a);
        const Card *cards = room->askForExchange(ran, "zhaoliao", ran->getCards("he").length(), 1, true, "@zhaoliao:" + a->objectName(), true);
        ran->tag.remove("zhaoliao_target");
        if (cards) {
            room->notifySkillInvoked(ran, objectName());
            QList<ServerPlayer *> logto;
            logto << a;
            room->touhouLogmessage("#ChoosePlayerWithSkill", ran, "zhaoliao", logto);

            room->obtainCard(a, cards, false);
            delete cards;
            cards = NULL;
            return true;
        }
        return false;
    }

    bool effect(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &data) const
    {

        ServerPlayer *player = data.value<DamageStruct>().to;
        ServerPlayer *ran = invoke->invoker;
        QString choice;
        ExNihilo *exnihilo = new ExNihilo(Card::SuitToBeDecided, -1);
#pragma message WARN("todo_Fs: port the getExpandPile() function from V2")

        if (player->isKongcheng()) {
            bool expand_pile = false;
            if (!player->getPile("wooden_ox").isEmpty())
                expand_pile = true;
            if (player->hasSkill("shanji") && !player->getPile("piao").isEmpty())
                expand_pile = true;
            if (!expand_pile)
                choice = "zhaoliao1";
        } else if (player->isCardLimited(exnihilo, Card::MethodUse, true))
            choice = "zhaoliao1";
        else
            choice = room->askForChoice(ran, objectName(), "zhaoliao1+zhaoliao2");

        if (choice == "zhaoliao1") {
            // memory leak!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            delete exnihilo;
            ran->drawCards(1);
        } else {
            if (!room->askForUseCard(player, "@@zhaoliao!", "zhaoliaouse")) {
                QList<int> handcards = player->handCards();
                handcards << player->getPile("wooden_ox");
                if (player->hasSkill("shanji"))
                    handcards << player->getPile("piao");
                exnihilo->addSubcard(handcards.value(qrand() % handcards.length()));
                exnihilo->setSkillName("_zhaoliao");
                room->useCard(CardUseStruct(exnihilo, player), true);
            } else
                delete exnihilo;
        }

        return false;
    }
};


class Jiaoxia : public TriggerSkill
{
public:
    Jiaoxia() : TriggerSkill("jiaoxia")
    {
        events << Dying;
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent, const Room *, const QVariant &data) const
    {
        ServerPlayer *who = data.value<DyingStruct>().who;
        if (who->hasSkill(this) && who->getEquips().length() > 0)
            return QList<SkillInvokeDetail>() << SkillInvokeDetail(this, who, who);
        return QList<SkillInvokeDetail>();
    }

    //default cost

    bool effect(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const
    {
        ServerPlayer *player = invoke->invoker;
        JudgeStruct judge;
        judge.reason = objectName();
        judge.who = player;
        judge.good = false;
        judge.pattern = ".|heart";

        room->judge(judge);
        if (judge.isGood()) {
            RecoverStruct recover;
            recover.recover = 1;
            recover.who = player;
            room->recover(player, recover);
        }
        return false;
    }
};



class Shuangren : public TargetModSkill
{
public:
    Shuangren() : TargetModSkill("shuangren")
    {
        frequency = NotFrequent;
        pattern = "Slash";
    }

    virtual int getExtraTargetNum(const Player *player, const Card *) const
    {
        if (player->hasSkill(objectName()))
            return 1;
        else
            return 0;
    }
};

class Youming : public TriggerSkill
{
public:
    Youming() :TriggerSkill("youming")
    {
        events << EventPhaseChanging;
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent, const Room *, const QVariant &data) const
    {
        PhaseChangeStruct change = data.value<PhaseChangeStruct>();
        ServerPlayer *player = change.player;
        if (player->hasSkill(this)) {
            if ((change.to == Player::Draw && !player->isSkipped(Player::Draw)) ||
                (change.to == Player::Play && !player->isSkipped(Player::Play)))
                return QList<SkillInvokeDetail>() << SkillInvokeDetail(this, player, player);
        }
        return QList<SkillInvokeDetail>();
    }

    bool cost(TriggerEvent, Room *, QSharedPointer<SkillInvokeDetail> invoke, QVariant &data) const
    {
        PhaseChangeStruct change = data.value<PhaseChangeStruct>();
        ServerPlayer *player = invoke->invoker;
        switch (change.to) {
            case Player::Draw:
            {
                return player->askForSkillInvoke(objectName(), "draw2play");
                break;
            }
            case Player::Play:
            {
                return player->askForSkillInvoke(objectName(), "play2draw");
                break;
            }
            default:
                return false;
        }
        return false;
    }


    bool effect(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &data) const
    {
        PhaseChangeStruct change = data.value<PhaseChangeStruct>();
        ServerPlayer *player = invoke->invoker;
        switch (change.to) {
            case Player::Draw:
            {
                player->skip(Player::Draw);
                Player::Phase phase = player->getPhase();
                player->setPhase(Player::Play);
                room->broadcastProperty(player, "phase");
                RoomThread *thread = room->getThread();
                QVariant playerVariant = QVariant::fromValue(player);
                if (!thread->trigger(EventPhaseStart, room, playerVariant))
                    thread->trigger(EventPhaseProceeding, room, playerVariant);
                thread->trigger(EventPhaseEnd, room, playerVariant);

                player->setPhase(phase);
                room->broadcastProperty(player, "phase");
                break;
            }
            case Player::Play:
            {
                player->skip(Player::Play);
                Player::Phase phase = player->getPhase();
                player->setPhase(Player::Draw);
                room->broadcastProperty(player, "phase");
                RoomThread *thread = room->getThread();
                QVariant playerVariant = QVariant::fromValue(player);
                if (!thread->trigger(EventPhaseStart, room, playerVariant))
                    thread->trigger(EventPhaseProceeding, room, playerVariant);
                thread->trigger(EventPhaseEnd, room, playerVariant);

                player->setPhase(phase);
                room->broadcastProperty(player, "phase");
                break;
            }
            default:
                return false;
        }

        return false;
    }
};



class XiezouVS : public ZeroCardViewAsSkill
{
public:
    XiezouVS() : ZeroCardViewAsSkill("xiezou")
    {
        response_pattern = "@@xiezou";
    }

    virtual const Card *viewAs() const
    {
        QString cardname = Self->property("xiezou_card").toString();
        Card *card = Sanguosha->cloneCard(cardname);

        card->setSkillName("xiezou");
        return card;
    }
};


class Xiezou : public TriggerSkill
{
public:
    Xiezou() : TriggerSkill("xiezou")
    {
        events << EventPhaseEnd << PreCardUsed << CardResponded << EventPhaseChanging;
        view_as_skill = new XiezouVS;
    }

    void record(TriggerEvent triggerEvent, Room *room, QVariant &data) const
    {
        if (triggerEvent == PreCardUsed || triggerEvent == CardResponded) {
            ServerPlayer *player = NULL;
            const Card * card = NULL;
            if (triggerEvent == PreCardUsed) {
                player = data.value<CardUseStruct>().from;
                card = data.value<CardUseStruct>().card;
            } else {
                CardResponseStruct response = data.value<CardResponseStruct>();
                player = response.m_from;
                if (response.m_isUse)
                    card = response.m_card;
            }
            if (player && player->getPhase() == Player::Play
                && card && card->getHandlingMethod() == Card::MethodUse)
                room->setPlayerProperty(player, "xiezou_card", card->objectName());
        } else if (triggerEvent == EventPhaseChanging) {
            PhaseChangeStruct change = data.value<PhaseChangeStruct>();
            if (change.from == Player::Play) {
                room->setPlayerProperty(change.player, "xiezou_card", QVariant());
            }
        }
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent triggerEvent, const Room *, const QVariant &data) const
    {
        if (triggerEvent == EventPhaseEnd) {
            ServerPlayer *player = data.value<ServerPlayer *>();
            if (player->getPhase() == Player::Play && player->hasSkill(this)) {
                QString cardname = player->property("xiezou_card").toString();
                Card *card = Sanguosha->cloneCard(cardname);
                if (card == NULL)
                    return QList<SkillInvokeDetail>();
                DELETE_OVER_SCOPE(Card, card)
                if (card->isKindOf("Slash") || card->isKindOf("Peach") || card->isNDTrick()) {
                    if (!player->isCardLimited(card, Card::MethodUse))
                        return QList<SkillInvokeDetail>() << SkillInvokeDetail(this, player, player);
                }
            }
        }
        return QList<SkillInvokeDetail>();
    }

    bool cost(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const
    {
        QString cardname = invoke->invoker->property("xiezou_card").toString();
        Card *card = Sanguosha->cloneCard(cardname);
        QString prompt = "@xiezou:" + card->objectName();
        delete card;
        room->askForUseCard(invoke->invoker, "@@xiezou", prompt);
        return false;
    }
};


class Hesheng : public TriggerSkill
{
public:
    Hesheng() : TriggerSkill("hesheng")
    {
        events << DamageInflicted;
        frequency = Compulsory;
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent, const Room *room, const QVariant &data) const
    {
        bool can = false;
        foreach (ServerPlayer *p, room->getAllPlayers()) {
            if (p->getCards("j").length() > 0) {
                can = true;
                break;
            }
        }
        DamageStruct damage = data.value<DamageStruct>();
        if (can && damage.to->hasSkill(this))
            return QList<SkillInvokeDetail>() << SkillInvokeDetail(this, damage.to, damage.to, NULL, true);
        return QList<SkillInvokeDetail>();
    }

    bool effect(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &data) const
    {
        DamageStruct damage = data.value<DamageStruct>();
        room->touhouLogmessage("#hesheng", invoke->invoker, "hesheng", QList<ServerPlayer *>(), QString::number(damage.damage));
        room->notifySkillInvoked(invoke->invoker, objectName());
        return true;
    }
};




class ZhanzhenVS : public OneCardViewAsSkill
{
public:
    ZhanzhenVS() : OneCardViewAsSkill("zhanzhen")
    {
        filter_pattern = "EquipCard";
        response_or_use = true;
    }

    virtual bool isEnabledAtPlay(const Player *player) const
    {
        return Slash::IsAvailable(player);
    }

    virtual bool isEnabledAtResponse(const Player *, const QString &pattern) const
    {
        return pattern == "jink" || pattern == "slash";
    }

    virtual const Card *viewAs(const Card *originalCard) const
    {
        if (originalCard != NULL) {
            QString pattern = Sanguosha->currentRoomState()->getCurrentCardUsePattern();
            if (pattern == "jink") {
                Jink *jink = new Jink(originalCard->getSuit(), originalCard->getNumber());
                jink->addSubcard(originalCard);
                jink->setSkillName(objectName());
                return jink;
            } else {
                Slash *slash = new Slash(originalCard->getSuit(), originalCard->getNumber());
                slash->addSubcard(originalCard);
                slash->setSkillName(objectName());
                return slash;
            }
        } else
            return NULL;
    }
};

class Zhanzhen : public TriggerSkill
{
public:
    Zhanzhen() : TriggerSkill("zhanzhen")
    {
        events << CardsMoveOneTime;
        view_as_skill = new ZhanzhenVS;
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent, const Room *room, const QVariant &data) const
    {
        CardsMoveOneTimeStruct move = data.value<CardsMoveOneTimeStruct>();
        ServerPlayer *player = qobject_cast<ServerPlayer *>(move.from);
        if (player != NULL && player->hasSkill(this) && move.card_ids.length() == 1 && move.to_place == Player::DiscardPile
            && ((move.reason.m_reason & CardMoveReason::S_MASK_BASIC_REASON) == CardMoveReason::S_REASON_USE
            || (move.reason.m_reason & CardMoveReason::S_MASK_BASIC_REASON) == CardMoveReason::S_REASON_RESPONSE)) {
            const Card *card = move.reason.m_extraData.value<const Card *>();
            if (card && card->getSkillName() == objectName() && room->getCardPlace(move.card_ids.first()) == Player::DiscardPile)
                return QList<SkillInvokeDetail>() << SkillInvokeDetail(this, player, player);
        }
        return QList<SkillInvokeDetail>();
    }

    bool cost(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &data) const
    {
        ServerPlayer *player = invoke->invoker;
        CardsMoveOneTimeStruct move = data.value<CardsMoveOneTimeStruct>();
        const Card *card = move.reason.m_extraData.value<const Card *>();
        const Card *realcard = Sanguosha->getEngineCard(move.card_ids.first());
        player->tag["zhanzhen"] = QVariant::fromValue(realcard);
        ServerPlayer *target = room->askForPlayerChosen(player, room->getOtherPlayers(player), objectName(),
            "@zhanzhen:" + card->objectName() + ":" + realcard->objectName(), true, true);
        if (target) {
            player->tag["zhanzhen_select"] = QVariant::fromValue(target);
            return true;
        }
        return false;
    }

    bool effect(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &data) const
    {
        CardsMoveOneTimeStruct move = data.value<CardsMoveOneTimeStruct>();
        ServerPlayer *player = invoke->invoker;
        ServerPlayer *target = player->tag.value("zhanzhen_select").value<ServerPlayer *>();
        player->tag.remove("zhanzhen_select");
        if (target) {
            CardsMoveStruct mo;
            mo.card_ids = move.card_ids;
            mo.to = target;
            mo.to_place = Player::PlaceHand;
            room->moveCardsAtomic(mo, true);
        }
        return false;
    }
};

class Renou : public TriggerSkill
{
public:
    Renou() : TriggerSkill("renou")
    {
        events << EventPhaseStart;
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent, const Room *, const QVariant &data) const
    {
        ServerPlayer *player = data.value<ServerPlayer *>();
        if (player->getPhase() == Player::Start && player->hasSkill(this))
            return QList<SkillInvokeDetail>() << SkillInvokeDetail(this, player, player);
        return QList<SkillInvokeDetail>();
    }

    //default cost

    bool effect(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const
    {
        QList<int> list = room->getNCards(5);
        ServerPlayer *player = invoke->invoker;
        CardsMoveStruct move(list, NULL, Player::PlaceTable, CardMoveReason(CardMoveReason::S_REASON_TURNOVER, invoke->invoker->objectName(), objectName(), QString()));
        room->moveCardsAtomic(move, true);

        QList<int> able;
        QList<int> disabled;
        foreach (int id, list) {
            Card *tmp_card = Sanguosha->getCard(id);
            if (tmp_card->isKindOf("EquipCard")) {
                able << id;
            } else {
                foreach (const Card *c, player->getCards("e")) {
                    if (c->getSuit() == tmp_card->getSuit()) {
                        disabled << id;
                        break;
                    }
                }
                if (!disabled.contains(id))
                    able << id;
            }
        }
        room->fillAG(list, NULL, disabled);
        int obtainId = -1;
        if (able.length() > 0) {
            obtainId = room->askForAG(player, able, true, objectName());
            if (obtainId > -1)
                room->obtainCard(player, obtainId, true);
        }

        room->getThread()->delay(1000);
        room->clearAG();

        //throw other cards
        if (obtainId > -1)
            list.removeOne(obtainId);
        if (!list.isEmpty()) {
            CardMoveReason reason(CardMoveReason::S_REASON_NATURAL_ENTER, invoke->invoker->objectName(), objectName(), QString());
            DummyCard dummy(list);
            room->throwCard(&dummy, reason, NULL);
        }
        return false;
    }
};



class Shishen : public TriggerSkill
{
public:
    Shishen() : TriggerSkill("shishen")
    {
        events << EventPhaseStart << Damaged;
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent triggerEvent, const Room *, const QVariant &data) const
    {
        ServerPlayer *player;
        if (triggerEvent == EventPhaseStart)
            player = data.value<ServerPlayer *>();
        else
            player = data.value<DamageStruct>().to;

        if (!player->hasSkill(this))
            return QList<SkillInvokeDetail>();

        if ((triggerEvent == EventPhaseStart &&  player->getPhase() == Player::Start) || triggerEvent == Damaged) {
            if (player->getMark("@shi") > 0)
                return QList<SkillInvokeDetail>() << SkillInvokeDetail(this, player, player);
        } else if (triggerEvent == EventPhaseStart &&  player->getPhase() == Player::Play) {
            if (player->getMark("@shi") == 0)
                return QList<SkillInvokeDetail>() << SkillInvokeDetail(this, player, player);
        }
        return QList<SkillInvokeDetail>();
    }

    bool cost(TriggerEvent triggerEvent, Room *, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const
    {
        ServerPlayer *player = invoke->invoker;
        if (player->getMark("@shi") > 0) {
            if (triggerEvent == Damaged)
                player->setFlags("shishen_choice");//for ai
            bool invoke = player->askForSkillInvoke(objectName());
            player->setFlags("-shishen_choice");
            if (invoke)
                return true;
        } else if (player->getMark("@shi") == 0) {
            return player->askForSkillInvoke(objectName());
        }
        return false;
    }

    bool effect(TriggerEvent, Room *, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const
    {
        if (invoke->invoker->getMark("@shi") > 0)
            invoke->invoker->loseMark("@shi");
        else
            invoke->invoker->gainMark("@shi");

        return false;
    }
};

class Yexing : public AttackRangeSkill
{
public:
    Yexing() : AttackRangeSkill("yexing")
    {

    }

    virtual int getExtra(const Player *target, bool) const
    {
        if (target->hasSkill(objectName()) && target->getMark("@shi") == 0)
            return 1;
        return 0;
    }
};

class YexingEffect : public TriggerSkill
{
public:
    YexingEffect() : TriggerSkill("#yexing")
    {
        frequency = Compulsory;
        events << GameStart << PreMarkChange << CardEffected << SlashEffected << EventAcquireSkill << EventLoseSkill << MarkChanged;
    }

    void record(TriggerEvent triggerEvent, Room *room, QVariant &data)
    {
        if (triggerEvent == CardEffected || triggerEvent == SlashEffected)
            return;

        ServerPlayer *player;
        //@todo: need adjust skill pingyi and function Player::hasSkill()
        if (triggerEvent == PreMarkChange) {
            MarkChangeStruct change = data.value<MarkChangeStruct>();
            if (change.name != "@shi")
                return;
            player = change.player;
            if (!player->hasSkill("yexing")) return;
            int mark = player->getMark("@shi");

            if (mark > 0 && (mark + change.num == 0) && player->getMark("yexing_limit") == 0) {
                room->setPlayerMark(player, "yexing_limit", 1);
                room->setPlayerCardLimitation(player, "use", "TrickCard+^DelayedTrick", false);
            } else if (mark == 0 && (mark + change.num > 0) && player->getMark("yexing_limit") > 0) {
                room->setPlayerMark(player, "yexing_limit", 0);
                room->removePlayerCardLimitation(player, "use", "TrickCard+^DelayedTrick$0");
            }
            return;
        }

        if (triggerEvent == GameStart) {
            player = data.value<ServerPlayer *>();
        } else if (triggerEvent == EventAcquireSkill || triggerEvent == EventLoseSkill) {
            player = data.value<SkillAcquireDetachStruct>().player;
        }
#pragma message WARN("todo_lwtmusou: to find a new method to record carduse limit while skill has been invailded, like skill changshi")
        else if (triggerEvent == MarkChanged) {
            player = data.value<MarkChangeStruct>().player;
        }

        if (player && player->hasSkill("yexing") && player->getMark("yexing_limit") == 0 && player->getMark("@shi") == 0) {
            room->setPlayerMark(player, "yexing_limit", 1);
            room->setPlayerCardLimitation(player, "use", "TrickCard+^DelayedTrick", false);
        } else if (player && player->getMark("yexing_limit") > 0 && (!player->hasSkill("yexing") || player->getMark("@shi") > 0)) {
            room->setPlayerMark(player, "yexing_limit", 0);
            room->removePlayerCardLimitation(player, "use", "TrickCard+^DelayedTrick$0");
        }
    }


    QList<SkillInvokeDetail> triggerable(TriggerEvent triggerEvent, const Room *, const QVariant &data) const
    {
        if (triggerEvent == SlashEffected) {
            SlashEffectStruct effect = data.value<SlashEffectStruct>();
            if (effect.nature == DamageStruct::Normal && effect.to->getMark("@shi") == 0 && effect.to->hasSkill("yexing"))
                return QList<SkillInvokeDetail>() << SkillInvokeDetail(this, effect.to, effect.to, NULL, true);
        }
        if (triggerEvent == CardEffected) {
            CardEffectStruct effect = data.value<CardEffectStruct>();
            if ((effect.card->isKindOf("SavageAssault") || effect.card->isKindOf("ArcheryAttack")) && effect.to->getMark("@shi") == 0
                && effect.to->hasSkill("yexing"))
                return QList<SkillInvokeDetail>() << SkillInvokeDetail(this, effect.to, effect.to, NULL, true);
        }
        return QList<SkillInvokeDetail>();
    }

    bool effect(TriggerEvent triggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &data) const
    {
        if (triggerEvent == SlashEffected) {
            SlashEffectStruct effect = data.value<SlashEffectStruct>();
            room->touhouLogmessage("#LingqiAvoid", effect.to, effect.slash->objectName(), QList<ServerPlayer *>(), "yexing");
            room->notifySkillInvoked(effect.to, "yexing");
            //room->setEmotion(effect.to, "armor/vine");
            return true;

        }
        if (triggerEvent == CardEffected) {
            CardEffectStruct effect = data.value<CardEffectStruct>();
            room->touhouLogmessage("#LingqiAvoid", effect.to, effect.card->objectName(), QList<ServerPlayer *>(), "yexing");
            room->notifySkillInvoked(effect.to, "yexing");
            //room->setEmotion(effect.to, "armor/vine");
            return true;
        }

        return false;
    }
};


class YaoshuVS : public ZeroCardViewAsSkill
{
public:
    YaoshuVS() : ZeroCardViewAsSkill("yaoshu")
    {
        response_pattern = "@@yaoshu";
    }

    virtual const Card *viewAs() const
    {
        QString cardname = Self->property("yaoshu_card").toString();
        Card *card = Sanguosha->cloneCard(cardname);

        card->setSkillName("yaoshu");
        return card;
    }
};

class Yaoshu : public TriggerSkill
{
public:
    Yaoshu() : TriggerSkill("yaoshu")
    {
        events << CardFinished;
        view_as_skill = new YaoshuVS;
    }

    void record(TriggerEvent, Room *room, QVariant &data) const
    {
        CardUseStruct use = data.value<CardUseStruct>();
        ServerPlayer *player = use.from;
        if (use.card->getSkillName() == "yaoshu")
            room->setPlayerProperty(player, "yaoshu_card", QVariant());
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent, const Room *, const QVariant &data) const
    {
        CardUseStruct use = data.value<CardUseStruct>();
        ServerPlayer *player = use.from;
        if (use.card && use.card->isNDTrick() && !use.card->isKindOf("Nullification")) {
            if (use.card->getSkillName() == "yaoshu")
                return QList<SkillInvokeDetail>();

            if (!use.m_isHandcard)
                return QList<SkillInvokeDetail>();
            //for turnbroken
            if (player->hasFlag("Global_ProcessBroken") || !player->hasSkill(this))
                return QList<SkillInvokeDetail>();

            const Card *c = Sanguosha->cloneCard(use.card->objectName());
            if (c == NULL)
                return QList<SkillInvokeDetail>();
            DELETE_OVER_SCOPE(const Card, c)
            if (!player->isCardLimited(c, Card::MethodUse))
                return QList<SkillInvokeDetail>() << SkillInvokeDetail(this, player, player);
        }
        return QList<SkillInvokeDetail>();
    }

    bool cost(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &data) const
    {
        CardUseStruct use = data.value<CardUseStruct>();
        ServerPlayer *player = invoke->invoker;
        room->setPlayerProperty(player, "yaoshu_card", use.card->objectName());
        room->askForUseCard(player, "@@yaoshu", "@yaoshu:" + use.card->objectName());
        return false;
    }
};

class Jiyi : public TriggerSkill
{
public:
    Jiyi() : TriggerSkill("jiyi")
    {
        events << TurnedOver;
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent, const Room *, const QVariant &data) const
    {
        ServerPlayer *player = data.value<ServerPlayer *>();
        if (player->hasSkill(this))
            return QList<SkillInvokeDetail>() << SkillInvokeDetail(this, player, player);
        return QList<SkillInvokeDetail>();
    }

    //default cost

    bool effect(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const
    {
        ServerPlayer *player = invoke->invoker;
        player->drawCards(2);
        if (player->isKongcheng())
            return false;
        QList<int> hc = player->handCards();
        room->askForRende(player, hc, objectName(), false, true, qMin(2, player->getHandcardNum()));

        return false;
    }
};

class Chunmian : public TriggerSkill
{
public:
    Chunmian() : TriggerSkill("chunmian")
    {
        frequency = Compulsory;
        events << EventPhaseStart;
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent, const Room *, const QVariant &data) const
    {
        ServerPlayer *player = data.value<ServerPlayer *>();
        if (player->getPhase() == Player::Finish && player->hasSkill(this))
            return QList<SkillInvokeDetail>() << SkillInvokeDetail(this, player, player, NULL, true);
        return QList<SkillInvokeDetail>();
    }


    bool effect(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const
    {
        ServerPlayer *player = invoke->invoker;
        room->touhouLogmessage("#TriggerSkill", player, objectName());
        room->notifySkillInvoked(player, objectName());
        player->turnOver();
        return false;
    }
};



class Baochun : public TriggerSkill
{
public:
    Baochun() : TriggerSkill("baochun")
    {
        events << Damaged;
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent, const Room *, const QVariant &data) const
    {
        DamageStruct damage = data.value<DamageStruct>();
        if (damage.to->hasSkill(this))
            return QList<SkillInvokeDetail>() << SkillInvokeDetail(this, damage.to, damage.to);
        return QList<SkillInvokeDetail>();
    }

    bool cost(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const
    {
        ServerPlayer *player = invoke->invoker;
        ServerPlayer *target = room->askForPlayerChosen(player, room->getAlivePlayers(), objectName(), "@" + objectName() + ":" + QString::number(player->getLostHp()), true, true);
        if (target) {
            invoke->invoker->tag["baochun_target"] = QVariant::fromValue(target);
            return true;
        }

        return false;
    }

    bool effect(TriggerEvent, Room *, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const
    {
        ServerPlayer *target = invoke->invoker->tag["baochun_target"].value<ServerPlayer *>();
        invoke->invoker->tag.remove("baochun_target");
        target->drawCards(invoke->invoker->getLostHp());
        return false;
    }
};


class Chunyi : public TriggerSkill
{
public:
    Chunyi() : TriggerSkill("chunyi")
    {
        frequency = Compulsory;
        events << EventPhaseStart;
    }
    QList<SkillInvokeDetail> triggerable(TriggerEvent, const Room *, const QVariant &data) const
    {
        ServerPlayer *player = data.value<ServerPlayer *>();
        if (player->getPhase() == Player::Start && player->hasSkill(this) && player->getMaxHp() < 6)
            return QList<SkillInvokeDetail>() << SkillInvokeDetail(this, player, player, NULL, true);
        return QList<SkillInvokeDetail>();
    }


    bool effect(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const
    {
        ServerPlayer *player = invoke->invoker;
        room->notifySkillInvoked(player, objectName());

        room->setPlayerProperty(player, "maxhp", player->getMaxHp() + 1);
        room->touhouLogmessage("#TriggerSkill", player, objectName());
        room->touhouLogmessage("#GainMaxHp", player, QString::number(1));
        room->touhouLogmessage("#GetHp", player, QString::number(player->getHp()), QList<ServerPlayer *>(), QString::number(player->getMaxHp()));
        return false;
    }
};


class Zhancao : public TriggerSkill
{
public:
    Zhancao() : TriggerSkill("zhancao")
    {
        events << TargetConfirmed;
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent, const Room *room, const QVariant &data) const
    {
        CardUseStruct use = data.value<CardUseStruct>();
        QList<SkillInvokeDetail> d;
        if (use.card->isKindOf("Slash")) {
            QList<ServerPlayer *> srcs = room->findPlayersBySkillName(objectName());
            foreach (ServerPlayer *p, srcs) {
                foreach (ServerPlayer *to, use.to) {
                    if (to->isAlive() && (p->inMyAttackRange(to) || p == to))
                        d << SkillInvokeDetail(this, p, p, NULL, false, to);
                }
            }
        }
        return d;
    }



    bool cost(TriggerEvent, Room *, QSharedPointer<SkillInvokeDetail> invoke, QVariant &data) const
    {
        ServerPlayer *player = invoke->invoker;
        CardUseStruct use = data.value<CardUseStruct>();
        ServerPlayer *to = invoke->preferredTarget;

        player->tag["zhancao_carduse"] = data;
        player->tag["zhancao_target"] = QVariant::fromValue(to);
        QString prompt = "target:" + use.from->objectName() + ":" + to->objectName();
        bool effect = player->askForSkillInvoke(this, prompt);
        player->tag.remove("zhancao_carduse");
        player->tag.remove("zhancao_target");
        return effect;
    }

    bool effect(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &data) const
    {
        ServerPlayer *player = invoke->invoker;
        CardUseStruct use = data.value<CardUseStruct>();
        use.nullified_list << invoke->targets.first()->objectName();
        data = QVariant::fromValue(use);
        room->doAnimate(QSanProtocol::S_ANIMATE_INDICATE, player->objectName(), invoke->targets.first()->objectName());
        if (room->askForCard(player, ".Equip", "@zhancao-discard", data, objectName()) == NULL)
            room->loseHp(player);
        return true;
    }
};



MocaoCard::MocaoCard()
{
}

bool MocaoCard::targetFilter(const QList<const Player *> &targets, const Player *to_select, const Player *Self) const
{
    if (!targets.isEmpty() || to_select == Self)
        return false;
    return !to_select->getEquips().isEmpty();
}
void MocaoCard::onEffect(const CardEffectStruct &effect) const
{
    Room *room = effect.to->getRoom();

    int card_id = room->askForCardChosen(effect.from, effect.to, "e", "mocao");
    room->obtainCard(effect.from, card_id);
    if (effect.to->isWounded())
        effect.to->drawCards(effect.to->getLostHp());
}


class Mocao : public ZeroCardViewAsSkill
{
public:
    Mocao() : ZeroCardViewAsSkill("mocao")
    {
    }

    virtual bool isEnabledAtPlay(const Player *player) const
    {
        return !player->hasUsed("MocaoCard");
    }

    virtual const Card *viewAs() const
    {
        return new MocaoCard;
    }
};



class Shenyin : public TriggerSkill
{
public:
    Shenyin() : TriggerSkill("shenyin")
    {
        events << DamageCaused;
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent, const Room *, const QVariant &data) const
    {
        DamageStruct damage = data.value<DamageStruct>();
        if (damage.from && damage.from->hasSkill(this) && !damage.to->isNude())
            return QList<SkillInvokeDetail>() << SkillInvokeDetail(this, damage.from, damage.from);
        return QList<SkillInvokeDetail>();
    }

    bool cost(TriggerEvent, Room *, QSharedPointer<SkillInvokeDetail> invoke, QVariant &data) const
    {
        DamageStruct damage = data.value<DamageStruct>();
        QVariant _data = QVariant::fromValue(damage.to);
        invoke->invoker->tag["shenyin_damage"] = data;
        return invoke->invoker->askForSkillInvoke(objectName(), _data);
    }

    bool effect(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &data) const
    {
        DamageStruct damage = data.value<DamageStruct>();
        ServerPlayer *to = damage.to;
        ServerPlayer *player = invoke->invoker;
        int to_throw = room->askForCardChosen(player, to, "he", objectName());
        player->addToPile("yin_mark", to_throw);
        if (!to->isNude()) {
            int to_throw1 = room->askForCardChosen(player, to, "he", objectName());
            player->addToPile("yin_mark", to_throw1);
        }
        return true;
    }
};

class Xijian : public TriggerSkill
{
public:
    Xijian() : TriggerSkill("xijian")
    {
        events << EventPhaseStart << Damaged;
    }

    static QStringList prohibitPiles()
    {
        QStringList names;
        names << "suoding_cards" << "wooden_ox";
        return names;
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent triggerEvent, const Room *room, const QVariant &data) const
    {  //check target 
        ServerPlayer  *target;
        if (triggerEvent == EventPhaseStart && room->getCurrent() && room->getCurrent()->getPhase() == Player::Finish)
            target = room->getCurrent();
        else if (triggerEvent == Damaged) {
            DamageStruct damage = data.value<DamageStruct>();
            if (damage.to->isAlive())
                target = damage.to;
        }
        if (!target)
            return QList<SkillInvokeDetail>();


        //check skillowner
        QList<ServerPlayer *> yukaris = room->findPlayersBySkillName(objectName());
        if (yukaris.isEmpty()) return QList<SkillInvokeDetail>();

        //check pile
        QList<SkillInvokeDetail> d;
        foreach (ServerPlayer *liege, room->getAllPlayers()) {
            foreach (QString pile, liege->getPileNames()) {
                if (liege->getPile(pile).length() > 0 && !prohibitPiles().contains(pile)) {
                    foreach (ServerPlayer *yukari, yukaris)
                        d << SkillInvokeDetail(this, yukari, yukari, NULL, false, target);
                    return d;
                }
            }
        }
        return d;
    }


    bool cost(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const
    {
        QList<ServerPlayer *> plist;
        ServerPlayer *player = invoke->preferredTarget;
        ServerPlayer *yukari = invoke->invoker;
        foreach (ServerPlayer *liege, room->getAllPlayers()) {
            foreach (QString pile, liege->getPileNames()) {
                if (liege->getPile(pile).length() > 0 && !prohibitPiles().contains(pile)) {
                    plist << liege;
                    break;
                }
            }
        }

        if (plist.length() > 0) {
            yukari->tag["xijian_target"] = QVariant::fromValue(player);
            ServerPlayer *player_haspile = room->askForPlayerChosen(yukari, plist, objectName(), "@xijian:" + player->objectName(), true, true);

            if (player_haspile) {
                QList<int> idlist;
                foreach (QString pile, player_haspile->getPileNames()) {
                    if (player_haspile->getPile(pile).length() > 0 && !prohibitPiles().contains(pile)) {
                        foreach (int id, player_haspile->getPile(pile))
                            idlist << id;
                    }
                }

                room->fillAG(idlist, yukari);
                int card_id = room->askForAG(yukari, idlist, true, objectName());
                yukari->tag.remove("xijian_target");
                room->clearAG(yukari);

                if (card_id > -1) {
                    yukari->tag["xijian_id"] = QVariant::fromValue(card_id);

                    bool can_open = false;
                    QString pile_name;
                    foreach (QString pile, player_haspile->getPileNames()) {
                        if (player_haspile->getPile(pile).contains(card_id)) {
                            pile_name = pile;
                            break;
                        }
                    }
                    if (pile_name != NULL && player_haspile->pileOpen(pile_name, yukari->objectName()))
                        can_open = true;
                    yukari->tag["xijian_visible"] = QVariant::fromValue(can_open);
                    return true;
                }
            }
            yukari->tag.remove("xijian_target");
        }
        return false;
    }

    bool effect(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const
    {
        ServerPlayer *player = invoke->targets.first();
        int card_id = invoke->invoker->tag["xijian_id"].toInt();
        bool visible = invoke->invoker->tag["xijian_visible"].toBool();
        invoke->invoker->tag.remove("xijian_id");
        invoke->invoker->tag.remove("xijian_visible");
        //need reason to slove the disambugation of the move of piao is
        //xijian a "piao" or using a "piao"
        CardMoveReason reason(CardMoveReason::S_REASON_GOTBACK, player->objectName(), objectName(), QString());
        room->obtainCard(player, Sanguosha->getCard(card_id), reason, visible);
        return false;
    }
};


class Youqu : public TriggerSkill
{
public:
    Youqu() : TriggerSkill("youqu")
    {
        frequency = Compulsory;
        events << EventPhaseStart;
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent, const Room *, const QVariant &data) const
    {
        ServerPlayer *player = data.value<ServerPlayer *>();
        if (!player->hasSkill(this))
            return QList<SkillInvokeDetail>();
        if ((player->getPhase() == Player::Start  && player->getPile("siling").length() >= 2) || player->getPhase() == Player::Finish)
            return QList<SkillInvokeDetail>() << SkillInvokeDetail(this, player, player, NULL, true);
        return QList<SkillInvokeDetail>();
    }

    bool effect(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const
    {
        ServerPlayer *player = invoke->invoker;
        if (player->getPhase() == Player::Start) {
            QList<int> sl = player->getPile("siling");
            room->notifySkillInvoked(player, objectName());

            room->touhouLogmessage("#TriggerSkill", player, objectName());

            CardsMoveStruct move(sl, player, player, Player::PlaceSpecial, Player::PlaceHand, CardMoveReason(CardMoveReason::S_REASON_UNKNOWN, QString()));
            room->moveCardsAtomic(move, false);

            room->touhouLogmessage("#silinggain", player, objectName(), QList<ServerPlayer *>(), QString::number(sl.length()));
            room->damage(DamageStruct("siling", player, player));

        } else if (player->getPhase() == Player::Finish) {
            room->notifySkillInvoked(player, objectName());
            room->touhouLogmessage("#TriggerSkill", player, objectName());
            QString choice = room->askForChoice(player, objectName(), "siling1+siling2+siling3");
            QList<int> list;
            if (choice == "siling1")
                list = room->getNCards(1);
            else if (choice == "siling2")
                list = room->getNCards(2);
            else if (choice == "siling3")
                list = room->getNCards(3);
            CardMoveReason reason(CardMoveReason::S_REASON_UNKNOWN, "", NULL, "siling", "");
            player->addToPile("siling", list, false, reason);
        }
        return false;

    }

};

class Wangwu : public TriggerSkill
{
public:
    Wangwu() : TriggerSkill("wangwu")
    {
        events << TargetConfirming;
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent, const Room *, const QVariant &data) const
    {
        CardUseStruct use = data.value<CardUseStruct>();
        QList<SkillInvokeDetail> d;
        if (use.card && (use.card->isKindOf("Slash") || use.card->isNDTrick())
            && (use.card->isRed() || use.card->isBlack())) {
            if (use.from && use.from->isAlive()) {
                foreach (ServerPlayer *p, use.to) {
                    if (p->hasSkill(this) && !p->getPile("siling").isEmpty())
                        d << SkillInvokeDetail(this, p, p);
                }
            }
        }
        return d;

    }

    bool cost(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &data) const
    {
        CardUseStruct use = data.value<CardUseStruct>();
        ServerPlayer *player = invoke->invoker;
        player->tag["wangwu_use"] = data;
        QString prompt = "invoke:" + use.from->objectName() + ":" + use.card->objectName();
        if (player->askForSkillInvoke(objectName(), prompt)) {
            QList<int> same;
            QList<int> disabled;
            QList<int> list = player->getPile("siling");
            foreach (int id, list) {
                if (Sanguosha->getCard(id)->sameColorWith(use.card))
                    same << id;
                else
                    disabled << id;
            }
            room->fillAG(list, player, disabled);
            int id = -1;
            if (same.isEmpty()) {
                //give a delay to avoid lacking "siling" shortage
                room->getThread()->delay(1000);
                room->clearAG(player);
            } else {
                player->tag["wangwu_card"] = QVariant::fromValue(use.card);
                id = room->askForAG(player, same, true, objectName());
                player->tag.remove("wangwu_card");
                room->clearAG(player);
                if (id > -1) {
                    CardMoveReason reason(CardMoveReason::S_REASON_REMOVE_FROM_PILE, QString(), NULL, objectName(), QString());
                    room->throwCard(Sanguosha->getCard(id), reason, NULL);
                    room->doAnimate(QSanProtocol::S_ANIMATE_INDICATE, player->objectName(), use.from->objectName());
                    player->drawCards(1);
                }
            }
            return id > -1;
        }
        return false;
    }

    bool effect(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &data) const
    {
        CardUseStruct use = data.value<CardUseStruct>();
        room->damage(DamageStruct("wangwu", invoke->invoker, use.from));
        return false;
    }
};


// #pragma message WARN("todo_lwtmusou: rewrite siyu, notice that skill records (flag, tag, marks, etc.) should be updated while siyu TurnBroken")
// Fs: should check in every skill, better write the most records clear into the eventphasechanging(to = notactive) event
// Fs: it's no need to check at here now, the extra turn is inserted after the whole round finished
class HpymSiyu : public TriggerSkill
{
public:
    HpymSiyu() : TriggerSkill("hpymsiyu")
    {
        events << PostHpReduced << EventPhaseEnd;
        frequency = Compulsory;
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent triggerEvent, const Room *, const QVariant &data) const
    {
        if (triggerEvent == PostHpReduced) {
            ServerPlayer *youmu = NULL;
            if (data.canConvert<DamageStruct>())
                youmu = data.value<DamageStruct>().to;
            else
                youmu = data.value<HpLostStruct>().player;

            if (youmu != NULL && youmu->isAlive() && youmu->hasSkill(this) && youmu->getHp() < 1 && !youmu->isCurrent())
                return QList<SkillInvokeDetail>() << SkillInvokeDetail(this, youmu, youmu, NULL, true);
        } else {
            ServerPlayer *youmu = data.value<ServerPlayer *>();
            if (youmu->getMark("siyuinvoke") > 0 && youmu->getPhase() == Player::Play && youmu->getHp() < 1)
                return QList<SkillInvokeDetail>() << SkillInvokeDetail(this, youmu, youmu, NULL, true);
        }

        return QList<SkillInvokeDetail>();
    }

    bool cost(TriggerEvent triggerEvent, Room *, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const
    {
        if (triggerEvent == EventPhaseEnd)
            invoke->invoker->removeMark("siyuinvoke");

        return true;
    }

    bool effect(TriggerEvent triggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const
    {
        if (triggerEvent == PostHpReduced) {
            if (!invoke->invoker->faceUp())
                invoke->invoker->turnOver();

            QList<const Card *> tricks = invoke->invoker->getJudgingArea();
            foreach (const Card *trick, tricks) {
                CardMoveReason reason(CardMoveReason::S_REASON_NATURAL_ENTER, invoke->invoker->objectName());
                room->throwCard(trick, reason, NULL);
            }

            invoke->invoker->addMark("siyuinvoke");
            room->touhouLogmessage("#TriggerSkill", invoke->invoker, objectName());
            room->notifySkillInvoked(invoke->invoker, objectName());

            room->touhouLogmessage("#touhouExtraTurn", invoke->invoker, objectName());

            invoke->invoker->gainAnExtraTurn();
            throw TurnBroken;

            return true; // prevent enterdying
        } else if (triggerEvent == EventPhaseEnd) {
            room->notifySkillInvoked(invoke->invoker, "hpymsiyu");
            room->enterDying(invoke->invoker, NULL);
        }

        return false;
    }
};

class Juhe : public TriggerSkill
{
public:
    Juhe() : TriggerSkill("juhe")
    {
        events << DrawNCards << AfterDrawNCards;
        frequency = Frequent;
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent triggerEvent, const Room *, const QVariant &data) const
    {
        DrawNCardsStruct draw = data.value<DrawNCardsStruct>();

        QList<SkillInvokeDetail> d;
        if (triggerEvent == DrawNCards) {
            if (!draw.player->hasSkill(this))
                d << SkillInvokeDetail(this, draw.player, draw.player);
        } else if (triggerEvent == AfterDrawNCards) {
            if (draw.player->hasFlag("juheUsed") && draw.player->getHp() > 0)
                d << SkillInvokeDetail(this, draw.player, draw.player, NULL, true);
        }
        return d;
    }

    bool cost(TriggerEvent triggerEvent, Room *, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const
    {
        if (triggerEvent == DrawNCards)
            return invoke->invoker->askForSkillInvoke(objectName());
        else
            invoke->invoker->setFlags("-juheUsed");
        return true;
    }

    bool effect(TriggerEvent triggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &data) const
    {
        ServerPlayer *player = invoke->invoker;
        if (triggerEvent == DrawNCards) {
            DrawNCardsStruct draw = data.value<DrawNCardsStruct>();
            draw.n = draw.n + 3;
            data = QVariant::fromValue(draw);
            room->setPlayerFlag(player, "juheUsed");
        } else if (triggerEvent == AfterDrawNCards)
            room->askForDiscard(player, "juhe", player->getHp(), player->getHp(), false, false, "juhe_discard:" + QString::number(player->getHp()));
        return false;
    }
};

class Shizhao : public TriggerSkill
{
public:
    Shizhao() : TriggerSkill("shizhao")
    {
        events << EventPhaseStart;
        frequency = Frequent;
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent, const Room *room, const QVariant &data) const
    {
        ServerPlayer *p = data.value<ServerPlayer *>();
        if (p->isAlive() && p->isWounded() && p->getPhase() == Player::Start) {
            QList<SkillInvokeDetail> d;
            foreach (ServerPlayer *ran, room->getAllPlayers()) {
                if (ran->hasSkill(this))
                    d << SkillInvokeDetail(this, ran, ran);
            }
            return d;
        }

        return QList<SkillInvokeDetail>();
    }

    bool effect(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &data) const
    {
        ServerPlayer *p = data.value<ServerPlayer *>();
        QList<int> card_ids = room->getNCards(p->getLostHp());
        room->returnToTopDrawPile(card_ids);

        room->fillAG(card_ids, invoke->invoker);
        int to_throw = room->askForAG(invoke->invoker, card_ids, true, objectName());
        room->clearAG(invoke->invoker);
        if (to_throw == -1)
            return false;

        CardMoveReason reason(CardMoveReason::S_REASON_PUT, invoke->invoker->objectName(), objectName(), QString());
        room->throwCard(Sanguosha->getCard(to_throw), reason, NULL);

        return false;
    }
};

// the skill's 2 "subskill"s are of the same timing, and have different effect, so we must split them

class Jixiong : public TriggerSkill
{
public:
    Jixiong() : TriggerSkill("jixiong")
    {
        events << CardsMoveOneTime;
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent, const Room *room, const QVariant &data) const
    {
        CardsMoveOneTimeStruct move = data.value<CardsMoveOneTimeStruct>();
        if (move.to_place != Player::DiscardPile)
            return QList<SkillInvokeDetail>();
        QList<SkillInvokeDetail> d;
        foreach (int id, move.card_ids) {
            const Card *card = Sanguosha->getCard(id);
            if (card != NULL && card->getSuit() == Card::Heart && card->getNumber() >= 11 && card->getNumber() <= 13) {
                foreach (ServerPlayer *p, room->getAllPlayers()) {
                    if (p->hasSkill(this))
                        d << SkillInvokeDetail(this, p, p);
                }
            }
        }
        return d;
    }

    bool effect(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const
    {
        room->recover(invoke->invoker, RecoverStruct());
        return false;
    }
};

class Jixiong2 : public TriggerSkill
{
public:
    Jixiong2() : TriggerSkill("#jixiong2")
    {
        events << CardsMoveOneTime;
    }

    QList<SkillInvokeDetail> triggerable(TriggerEvent, const Room *room, const QVariant &data) const
    {
        CardsMoveOneTimeStruct move = data.value<CardsMoveOneTimeStruct>();
        if (move.to_place != Player::DiscardPile)
            return QList<SkillInvokeDetail>();

        ServerPlayer *dying = room->getCurrentDyingPlayer();
        if (dying != NULL && dying->isAlive())
            return QList<SkillInvokeDetail>();

        ServerPlayer *current = room->getCurrent();
        if (current == NULL || current->isDead() || !current->isCurrent())
            return QList<SkillInvokeDetail>();

        QList<SkillInvokeDetail> d;
        foreach (int id, move.card_ids) {
            const Card *card = Sanguosha->getCard(id);
            if (card != NULL && card->getSuit() == Card::Spade && card->getNumber() >= 11 && card->getNumber() <= 13) {
                foreach (ServerPlayer *p, room->getAllPlayers()) {
                    if (p->hasSkill(this))
                        d << SkillInvokeDetail(this, p, p, NULL, false, current);
                }
            }
        }
        return d;
    }

    bool cost(TriggerEvent, Room *, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const
    {
        return invoke->invoker->askForSkillInvoke("jixiong", QVariant::fromValue(invoke->preferredTarget));
    }

    bool effect(TriggerEvent, Room *room, QSharedPointer<SkillInvokeDetail> invoke, QVariant &) const
    {
        room->damage(DamageStruct("jixiong", invoke->invoker, invoke->targets.first()));
        return false;
    }
};


TH07Package::TH07Package()
    : Package("th07")
{
    General *yuyuko = new General(this, "yuyuko$", "yym", 4, false);
    yuyuko->addSkill(new Sidie);
    yuyuko->addSkill(new Wangxiang);

    General *yukari = new General(this, "yukari", "yym", 3, false);
    yukari->addSkill(new Jingjie);
    yukari->addSkill(new Sisheng);
    yukari->addSkill(new Jingdong);

    General *ran = new General(this, "ran", "yym", 3, false);
    ran->addSkill(new Zhaoliao);
    ran->addSkill(new Jiaoxia);

    General *youmu = new General(this, "youmu", "yym", 4, false);
    youmu->addSkill(new Shuangren);
    youmu->addSkill(new Youming);

    General *prismriver = new General(this, "prismriver", "yym", 3, false);
    prismriver->addSkill(new Xiezou);
    prismriver->addSkill(new Hesheng);

    General *alice = new General(this, "alice", "yym", 4, false);
    alice->addSkill(new Zhanzhen);
    alice->addSkill(new Renou);

    General *chen = new General(this, "chen", "yym", 3, false);
    chen->addSkill(new Shishen);
    chen->addSkill(new Yexing);
    chen->addSkill(new YexingEffect);
    chen->addSkill(new Yaoshu);
    related_skills.insertMulti("yexing", "#yexing");

    General *letty = new General(this, "letty", "yym", 4, false);
    letty->addSkill(new Jiyi);
    letty->addSkill(new Chunmian);

    General *lilywhite = new General(this, "lilywhite", "yym", 3, false);
    lilywhite->addSkill(new Baochun);
    lilywhite->addSkill(new Chunyi);

    General *shanghai = new General(this, "shanghai", "yym", 3, false);
    shanghai->addSkill(new Zhancao);
    shanghai->addSkill(new Mocao);

    General *yukari_sp = new General(this, "yukari_sp", "yym", 4, false);
    yukari_sp->addSkill(new Shenyin);
    yukari_sp->addSkill(new Xijian);

    General *yuyuko_sp = new General(this, "yuyuko_sp", "yym", 3, false);
    yuyuko_sp->addSkill(new Youqu);
    yuyuko_sp->addSkill(new Wangwu);

    General *youmu_slm = new General(this, "youmu_slm", "yym", 2, false);
    youmu_slm->addSkill(new HpymSiyu);
    youmu_slm->addSkill(new Juhe);

    General *ran_sp = new General(this, "ran_sp", "yym", 4, false);
    ran_sp->addSkill(new Shizhao);
    ran_sp->addSkill(new Jixiong);
    ran_sp->addSkill(new Jixiong2);
    related_skills.insertMulti("jixiong", "#jixiong2");

    General *youki = new General(this, "youki", "yym", 4, false);
    Q_UNUSED(youki);

    General *leira = new General(this, "leira", "yym", 4, false);
    Q_UNUSED(leira);

    addMetaObject<MocaoCard>();

    skills << new ZhaoliaoVS;
}

ADD_PACKAGE(TH07)

