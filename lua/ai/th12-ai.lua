--【普度】ailocal pudu_skill = {}pudu_skill.name = "pudu"table.insert(sgs.ai_skills, pudu_skill)pudu_skill.getTurnUseCard = function(self)	if self.player:hasUsed("puduCard") then return nil end	if self.player:getHp()<3 then return nil end	for _,p in sgs.qlist(self.room:getOtherPlayers(self.player)) do		if  p:isWounded() and self:isFriend(p) then			t=true		end	end	if t then	--return sgs.Card_Parse("#pudu:.:")	return sgs.Card_Parse("@puduCard=.")	end	return nilendsgs.ai_skill_use_func.puduCard=function(card,use,self)--sgs.ai_skill_use_func["#pudu"]=function(card,use,self)	self:sort(self.friends_noself,"hp")    for _, p in ipairs(self.friends_noself) do        if p:isWounded() then			use.card = card			if use.to then				use.to:append(p)				if use.to:length() >= 1 then return end			end		end    endendsgs.ai_card_intention.puduCard = -100--【救赎】aisgs.ai_skill_invoke.jiushu = true--【法华】ailocal fahua_loyalist=function(self,from,to,card)	if self:touhouEffectNullify(card,from,to) then 		return true	end	for _, askill in sgs.qlist(to:getVisibleSkillList()) do		local s_name = askill:objectName()--需要check技能无效？		local filter = sgs.ai_trick_prohibit[s_name]		if filter and type(filter) == "function" 			and filter(self, from, to, card) then			return true		end	end	return falseend--暂时定为true 让忠卖血去吧。。。 需要调整  头上已经有乐的时候，忠对你拆，结果给法华了。。。sgs.ai_skill_invoke.fahua = truesgs.ai_skill_invoke.fahua_change = function(self,data)	local lord=self.room:getTag("fahua_target"):toPlayer()	local use=self.room:getTag("fahua_use"):toCardUse()	if not self:isFriend(lord) then		return false	end	if self.player:isChained() and use.card:isKindOf("IronChain") then		return true	end	if fahua_loyalist(self,use.from,self.player,use.card) then		return true	end	local lieges = self.room:getLieges("xlc",lord)	self.room:sortByActionOrder(lieges)	fahua_invoked=true	for _, liege in sgs.qlist(lieges) do		if liege:objectName()== self.player:objectName() then			fahua_invoked=false			continue		end		if not fahua_invoked and self:isFriend(lord,liege) then			if  fahua_loyalist(self,use.from,liege,use.card) then				return false			end		end	end	--if self:hasTrickEffective(use.card, self.player, use.from) then 	--	return true 	--end	--if self:hasSkill("weizhuang") and use.from:isKongcheng() then	--	return true	--end		if self:isWeak(lord) then		return true	elseif not self:isWeak(self.player) then		return true	end	return falseendsgs.ai_choicemade_filter.skillInvoke.fahua_change = function(self, player, promptlist)	local target=self.room:getTag("fahua_target"):toPlayer()	if target then		if promptlist[#promptlist] == "yes" then			sgs.updateIntention(player, target, -60)		end	endendsgs.ai_trick_prohibit.fahua = function(self, from, to, card)	if not to:hasLordSkill("fahua") then return false end	local lieges = self.room:getLieges("xlc", to)	for _, liege in sgs.qlist(lieges) do		if self:isFriend(liege, to) then			if fahua_loyalist(self,from,liege,card) then				return true			end		end	end	return falseend--【未知】ai--你妹  真要考虑起来比制衡还坑爹。。。local weizhi_skill = {}weizhi_skill.name = "weizhi"table.insert(sgs.ai_skills, weizhi_skill)function weizhi_skill.getTurnUseCard(self)    if self.player:hasUsed("weizhiCard") then return nil end    local handcards = sgs.QList2Table(self.player:getHandcards())    if #handcards==0 then return nil end	self:sortByUseValue(handcards)	changes={}	for _,c in pairs (handcards) do		if not c:isKindOf("TrickCard") then			table.insert(changes,c)		end	end	if #changes>1 then		use_cards={}		table.insert(use_cards,changes[1]:getId())		table.insert(use_cards,changes[2]:getId())		--return sgs.Card_Parse("#weizhi:" .. table.concat(use_cards, "+") .. ":")		return sgs.Card_Parse("@weizhiCard=" .. table.concat(use_cards, "+") )	end	return nilendsgs.ai_skill_use_func.weizhiCard=function(card,use,self)	use.card = cardendsgs.ai_use_value.weizhiCard =  6sgs.ai_use_priority.weizhiCard = 6sgs.ai_cardneed.weizhi = function(to, card, self)	if not self:willSkipPlayPhase(to) then		return  not card:isKindOf("TrickCard")	endend--【伪装】aisgs.ai_skill_cardask["@weizhuang-discard"] = function(self, data)    local target =self.player:getTag("weizhuang_target"):toPlayer()	local use =data:toCardUse()	if not self:isEnemy(target) and not self:isFriend(target) then return "." end		effect=false--此bool值代表是否想让其生效	if self:isFriend(target)  then		if ("snatch|dismantlement"):match(use.card:objectName()) 		and target:getCards("j"):length()>0 		then			effect = true		end		if use.card:isKindOf("GodSalvation") and target:isWounded() then			effect = true		end	end	--对友方Nue需要弃牌使其有效的牌只有五谷和受伤时的桃园？还有兵乐？？	if self:isEnemy(target) then		effect=true		if use.card:isKindOf("AmazingGrace") then			effect = false		end		if use.card:isKindOf("GodSalvation")  then			effect = false		end	end	if not effect then return "." end	local cards = sgs.QList2Table(self.player:getCards("h"))    cards1={}	for _,card in pairs(cards) do		if card:isKindOf("BasicCard") then			table.insert(cards1,card)		end	end	if #cards1==0 then return "." end	self:sortByCardNeed(cards1)	    return "$" .. cards1[1]:getId()endsgs.ai_trick_prohibit.weizhuang = function(self, from, to, card)	if  card:isNDTrick() then		basics={"Jink","Slash","Peach","Analeptic"}		basic_num=0		for _,str in pairs (basics) do			basic_num=basic_num+ getCardsNum(str, from, self.player) 			if basic_num>0 then				return false			end		end		return true	end	return falseend--【净化】ai--part1：净化选择目标sgs.ai_skill_playerchosen.jinghua = function(self, targets)	--part1.1：获取有延时锦囊的玩家	--并将当前回合人（友方）暂定为净化的对象	local current=self.player:getRoom():getCurrent()	local list1={}--乐（友方）	local list2={}--兵(友方)	local list3={}--闪电（若需要拆电，敌我都算.若不需要，敌我都不考虑）	local jinghua_target	--非无损净化时要考虑血量	if current:objectName()~=self.player:objectName() 		and self.player:getHp()<2 then return nil end		for _,target in sgs.qlist(targets) do			if target:getCards("j"):length()>0  then			for _,card in sgs.qlist(target:getCards("j")) do				if card:isKindOf("Indulgence") and self:isFriend(target) then						table.insert(list1,target)						if current:objectName()==target:objectName() then							jinghua_target=target						end				elseif card:isKindOf("SupplyShortage") and self:isFriend(target) then						table.insert(list2,target)						if current:objectName()==target:objectName() then							jinghua_target=target						end				elseif  card:isKindOf("Lightning") and not self:invokeTouhouJudge() then							table.insert(list3,target)						if current:objectName()==target:objectName() then							jinghua_target=target						end				end			end			end	end	--场上无延时锦囊(程序里这种情况应该是不会询问技能发动的，所以此行可删)	if #list1==0 and  #list2==0 and #list3==0 then return nil end		--part1.2:敲定一个净化对象	--若当前回合人不需要净化,那么选择一个其他可能的对象	--这里优先考虑有两个延时锦囊的	if not jinghua_target then		for _,target in sgs.qlist(targets) do				if (table.contains(list1,target) and table.contains(list2,target)) or 			(table.contains(list1,target) and table.contains(list3,target))			or 	(table.contains(list2,target) and table.contains(list3,target)) then				jinghua_target=target			end		end	end		--没有同时顶着两个延时的人的话，且自己回合	--那么按照锦囊的重要度选一个对象  闪电》兵》乐 （其实更应该按座位？）	if not jinghua_target then		if current:objectName()==self.player:objectName() then			if #list3>0 then				jinghua_target=list3[1]			end			if #list2>0 then				jinghua_target=list2[1]			end			if #list1>0 then				jinghua_target=list1[1]			end		end	end		--终于敲定了一个净化对象之后,检测一下翻面	--翻面中提前解除无意义	--记录好一张延时锦囊的id（粗糙。。。有细化的可能）	if jinghua_target and jinghua_target:faceUp() then		for _,card in sgs.qlist(jinghua_target:getCards("j")) do			if card:isKindOf("SupplyShortage") or card:isKindOf("Indulgence") 			or (card:isKindOf("Lightning") and not self:invokeTouhouJudge())then				self.player:setTag("jinghua_id",sgs.QVariant(card:getId()))				break			end		end		return jinghua_target	else		return nil	endend--part2：净化选择卡牌sgs.ai_skill_cardchosen.jinghua = function(self, who, flags)	--直接选取之前预存的卡牌id	local id =self.player:getTag("jinghua_id"):toInt()	self.player:removeTag("jinghua_id")	return sgs.Sanguosha:getCard(id)end--part3:净化没有发动的情况下的仇恨sgs.ai_no_playerchosen_intention.jinghua =function(self, from)	--老虎自己的回合本来可以无损拆乐，却没有净化任何人。	--主公有兵乐，增加老虎对主公的仇恨	--ps 超魔里沙这种例外没考虑。。。因为存在爆衣对策，ai也不会对魔里沙用延时锦囊	--另外，队友主动对爆衣贴兵乐的ai是没有的	local lord =self.room:getLord()	if lord and from:getPhase() ~=sgs.Player_NotActive then		if lord:containsTrick("indulgence") or lord:containsTrick("supply_shortage") then			sgs.updateIntention(from, lord, 10)		end	endend--part4：净化选择了一张卡牌时的仇恨--等同于使用过拆 拆掉一张牌的仇恨 于是就不需要我们具体写了--ps 拆掉闪电也是有仇恨的哦 这个我是写在过拆的仇恨里的sgs.ai_choicemade_filter.cardChosen.jinghua = sgs.ai_choicemade_filter.cardChosen.dismantlement--part5：供净化的对策ai调用 sgs.ai_trick_prohibit.jinghua = function(self, from, to, card)	--非延时类不禁止	if not card:isKindOf("DelayedTrick")  then return false end	--友军不禁止 极端的例子 没有乐的目标了，但是友军有【诵经】，可以乐一下老虎嘛	if self:isFriend(from,to) then return false end	--其余情况，禁止对【净化】持有者使用锦囊	return trueend--【正义】ai--function SmartAI:hasTrickEffective(card, to, from)--[[sgs.ai_skill_invoke.zhengyi = function(self,data)	local use=self.room:getTag("zhengyi_use"):toCardUse()	luanying=self.room:findPlayerBySkillName("luanying")	weiya_current=self.room:getCurrent()	if use.card:isKindOf("Slash") then		if self:isEnemy(use.from)  and use.from:hasWeapon("Axe") and use.from:getCards("he"):length()>=2 then			return false		end		if self:hasWeiya() then			return false		end		if luanying and self:isEnemy(luanying) and luanying:getPile("jingjie"):length()>0 then			return false		end		if self:getCardsNum("Jink") > 0 then			return true		end		if use.from:hasSkill("sidie") and self:isFriend(use.from)  then			return true		end	end	if use.card:isKindOf("GlobalEffect") then		return true	end	if use.card:isKindOf("IronChain") then		return self.player:isChained()	end	if use.from:hasSkill("lizhi") and self:isFriend(use.from)  then		if use.card:isKindOf("Slash") or use.card:isKindOf("SavageAssault")  then			return true		end	end	if use.card:isKindOf("SavageAssault") and self:getCardsNum("Slash") > 1 then		if self:hasWeiya() then			return false		end		if luanying and self:isEnemy(luanying) and luanying:getPile("jingjie"):length()>0 then			return false		end		return true	end	return false end]]--[[sgs.ai_slash_prohibit.zhengyi = function(self, from, to, card)	if not card:isBlack() then 		return false	else		if from:hasSkill("shende") and self:getOverflow(from,from:getMaxCards())>0 then			return false		end	end	return trueend]]sgs.ai_cardneed.zhengyi = function(to, card, self)	--此时应该是没有被锁定技转化	return  card:isKindOf("Armor")end--【船难】aisgs.ai_skill_invoke.chuannan = function(self,data)	return trueendsgs.ai_skill_use["@@chuannan"] = function(self, prompt)	local damage=self.player:getTag("chuannan_damage"):toDamage()	local target 	if damage.to:objectName() == self.player:objectName() then 		target =damage.from	else		target =damage.to	end	if not target then return "." end	if self:isFriend(target) then return "." end	local card = sgs.cloneCard("supply_shortage")	if self:touhouDelayTrickBadTarget(card,target,self.player) then		return "."	end	local cards = self.player:getCards("h")	cards=self:touhouAppendExpandPileToList(self.player,cards)    cards = sgs.QList2Table(cards)	self:sortByUseValue(cards,true)	if #cards==0 then return "." end	return "@chuannanCard=".. cards[1]:getId() .."->" .. target:objectName()end--【理智】ai--SmartAI:getAoeValue(card, player)--sgs.ai_skill_cardask.aoesgs.ai_skill_invoke.lizhi = function(self,data)	local d = self.player:getTag("lizhi_damage"):toDamage()		local target=data:toPlayer()	if self:isFriend(target) then		return true	end	if self:isEnemy(target) then		if self:isWeak(target) then			return false		end		if d.damage>=2 then 			return false		end		if not self:canAttack(target) then			return true		end	endendsgs.ai_choicemade_filter.skillInvoke.lizhi = function(self, player, promptlist)	local d = self.player:getTag("lizhi_damage"):toDamage()	if d and d.to then		if promptlist[#promptlist] == "yes" then			if self:isEnemy(player, d.to) then				--有时候不能打卖血流  这个情况还得细分			else				sgs.updateIntention(player, d.to, -20)			end		elseif promptlist[#promptlist] == "no" then			sgs.updateIntention(player, d.to, 60)		end	endendsgs.ai_cardneed.lizhi = function(to, card, self)	if not self:willSkipPlayPhase(to) then		return  card:isKindOf("AOE")	endend--【云上】ai锁定技 不需要--SmartAI:hasTrickEffective--【搜集】aisgs.ai_skill_invoke.souji = true--【探索】aisgs.ai_skill_invoke.tansuo = true--【遗忘】aisgs.ai_skill_playerchosen.yiwang = function(self, targets)	local target_table= sgs.QList2Table(targets)    self:sort(target_table,"hp")	for _,p in pairs (target_table) do		if self:isFriend(p) then			return p		end	end	return nilendsgs.ai_playerchosen_intention.yiwang = -70sgs.yiwang_keep_value = { 	EquipCard = 7}sgs.ai_cardneed.yiwang = function(to, card, self)		return  card:isKindOf("EquipCard")end--【惊吓】ai--决策判断较麻烦。。。sgs.ai_skill_choice.jingxia=function(self)	local damage=self.player:getTag("jingxia"):toDamage()	local from=damage.from	local fieldcard=sgs.SPlayerList()	local fieldcard1=sgs.SPlayerList()	for _, p in sgs.qlist(self.room:getAllPlayers()) do        if self:isEnemy(p) then			if self.player:canDiscard(p, "e")   then 				fieldcard:append(p) 			end		end		if self:isFriend(p) then			if self.player:canDiscard(p, "j") and not p:containsTrick("lightning") then				fieldcard1:append(p) 			end		end		if p:containsTrick("lightning") and  not self:invokeTouhouJudge() then			fieldcard1:append(p) 		end	end	if self:isWeak(self.player) and self.player:canDiscard(self.player, "e") then		return "discardfield"	end	if fieldcard1:length()>0 then return "discardfield" end	if fieldcard:length()>1 then return "discardfield" end	if from and self:isEnemy(from) and not from:isNude() and  self.player:canDiscard(from, "he") then		return "discard"	end	return "dismiss"endsgs.ai_skill_playerchosen.jingxia = function(self, targets)	local fieldcard={}	local fieldcard1={}	for _, p in sgs.qlist(targets) do         if self:isEnemy(p) then			if self.player:canDiscard(p, "e")   then 				table.insert(fieldcard,p) 			end		end		if self:isFriend(p) then			if self.player:canDiscard(p, "j") and not p:containsTrick("lightning") then				table.insert(fieldcard1,p)  			end		end		if p:containsTrick("lightning") and  not self:invokeTouhouJudge() then			table.insert(fieldcard1,p)  		end	end		if self:isWeak(self.player) and self.player:getCards("e"):length()>0   and self.player:canDiscard(self.player, "e") then		return self.player	end		if #fieldcard1>0 then return fieldcard1[1] end	if #fieldcard > 0 then		self:sort(fieldcard,"hp")		return fieldcard[1]	end	return nilendsgs.ai_skill_cardchosen.jingxia = function(self, who, flags)	if self:isFriend(who) then		cards=who:getCards("j")		cards = sgs.QList2Table(cards)		for _,c in pairs (cards) do			if c:isKindOf("Lightning")  then				if not self:invokeTouhouJudge() then					return c				end			else				return c			end		end		cards=who:getCards("e")		cards = sgs.QList2Table(cards)		if #cards>0 then			return cards[1]		end	end	if self:isEnemy(who) then		if flags:match("j") then			cards=who:getCards("j")			cards = sgs.QList2Table(cards)			for _,c in pairs (cards) do				if c:isKindOf("Lightning")  then					if not self:invokeTouhouJudge() then						return c					end				end			end		end		cards=who:getCards("e")		cards = sgs.QList2Table(cards)		if #cards>0 then			return cards[1]		end		return sgs.Sanguosha:getCard(self:getCardRandomly(who, flags))		endend--sgs.ai_playerchosen_intention.jingxia = 50sgs.ai_choicemade_filter.cardChosen.jingxia = sgs.ai_choicemade_filter.cardChosen.dismantlement--【变幻】aisgs.ai_skill_invoke.bianhuan = function(self, data)	local damage =data:toDamage()	local x=self.player:getLostHp()	if damage.damage>1 or x>1 then		return true	end	if self.player:getHp()<2 and x>0 then		return true	end	return falseend--【神拳】ai--【怒火】ailocal nuhuo_skill = {}nuhuo_skill.name = "nuhuo"table.insert(sgs.ai_skills, nuhuo_skill)nuhuo_skill.getTurnUseCard = function(self)	if self.player:hasUsed("nuhuoCard") then return nil end	local slash = sgs.cloneCard("slash", sgs.Card_NoSuit, 0)	    slash:deleteLater()    if self.player:isCardLimited(slash,sgs.Card_MethodUse) then return nil end 			--return sgs.Card_Parse("#nuhuo:.:")	return sgs.Card_Parse("@nuhuoCard=.")endsgs.ai_skill_use_func.nuhuoCard=function(card,use,self)--sgs.ai_skill_use_func["#nuhuo"]=function(card,use,self)	local slash = sgs.cloneCard("slash", sgs.Card_NoSuit, 0)	--可以用dummyuse更加简洁？？	local targets={}--enemy	local targets1={}--friend	local others=self.room:getOtherPlayers(self.player)	for _,p in sgs.qlist(others) do		if  self.player:inMyAttackRange(p) 		and self:canAttack(p) and self.player:canSlash(p,slash,true)  then			local fakeDamage=sgs.DamageStruct()			fakeDamage.card=slash			fakeDamage.nature= self:touhouDamageNature(slash,self.player,p)			fakeDamage.damage=1			fakeDamage.from=self.player			fakeDamage.to=p				if p:hasSkill("xuying") or self:touhouDamage(fakeDamage,self.player,p).damage>0   or self:touhouDamageEffect(fakeDamage,self.player,p) then				if self:isEnemy(p) then					table.insert(targets,p)				end				if self:isFriend(p) then					table.insert(targets1,p)				end			end		end	end		if #targets==0 then return  end	--搭配一轮    for _,p in ipairs(self.friends_noself) do        if p:hasSkill("lizhi") then			use.card = card			if use.to then				use.to:append(p)				if use.to:length() >= 1 then return end			end		end    end	if self.player:getHp()<2 then return false end 	--self:sort(self.enemies,"hp")	if #self.friends_noself <1 and #targets1>0 then return false end 	local chooser = #self.friends_noself>0 and self.friends_noself[1] or others:first()	use.card = card	if use.to then		use.to:append(chooser)		if use.to:length() >= 1 then return end	endendsgs.ai_skill_playerchosen.nuhuo = function(self, targets)	local slash = sgs.cloneCard("slash", sgs.Card_NoSuit, 0)	local source=self.room:getCurrent()	local target_table= sgs.QList2Table(targets)    self:sort(target_table,"hp")	for _,p in pairs (target_table) do		if self:isEnemy(p) and self:canAttack(p,source) then			local fakeDamage=sgs.DamageStruct()			fakeDamage.card=slash			fakeDamage.nature= self:touhouDamageNature(slash,source,p)			fakeDamage.damage=1			fakeDamage.from=source			fakeDamage.to=p			if p:hasSkill("xuying") or self:touhouDamage(fakeDamage,source,p).damage>0   or self:touhouDamageEffect(fakeDamage,source,p) then				return p			end		end	end	--不能在这里加无奈选人的tag	--因为怒火选人的策略和选人后仇恨判断属于不同的人 此信息不应该对不同的人公开	--需要仇恨判断自己重新确认是否此次怒火选人的意图	return target_table[1]endsgs.ai_playerchosen_intention.nuhuo =function(self, from, to)	local source=self.room:getCurrent()	local slash = sgs.cloneCard("slash")	for _,p in sgs.qlist(self.room:getOtherPlayers(source)) do		if source:canSlash(p,slash,true) and not self:isFriend(p,to) then			-- and self:canAttack(p,source)			local fakeDamage=sgs.DamageStruct()			fakeDamage.card=slash			fakeDamage.nature= self:touhouDamageNature(slash,source,p)			fakeDamage.damage=1			fakeDamage.from=source			fakeDamage.to=p			--说明本来可以选择其他敌对角色			if p:hasSkill("xuying") or self:touhouDamage(fakeDamage,source,p).damage>0   or self:touhouDamageEffect(fakeDamage,source,p) then				sgs.updateIntention(from, to, 10)				break			end		end	endend--嘲讽值设定--[[sgs.ai_chaofeng.xlc001 = 3sgs.ai_chaofeng.xlc002 = 0sgs.ai_chaofeng.xlc003 = -2sgs.ai_chaofeng.xlc004 = -1sgs.ai_chaofeng.xlc005 = 0sgs.ai_chaofeng.xlc006 = 1sgs.ai_chaofeng.xlc007 = -1sgs.ai_chaofeng.xlc008 = 1]]