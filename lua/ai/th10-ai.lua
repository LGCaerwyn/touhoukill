

sgs.ai_skill_invoke.shende = true
sgs.ai_skill_discard.shende = function(self)
	local to_discard = {}
	local cards = self.player:getHandcards()
	cards = sgs.QList2Table(cards)
	self:sortByKeepValue(cards)

	table.insert(to_discard, cards[1]:getEffectiveId())

	return to_discard
end


local shende_skill = {}
shende_skill.name = "shende"
table.insert(sgs.ai_skills, shende_skill)
function shende_skill.getTurnUseCard(self)
    if self.player:getPile("shende"):length() < 2 then return nil end
	if not self.player:isWounded() then return nil end
	local need_useshende=false
	if self:cautionChangshi()  then 
		need_useshende=true
	end
	if  (self:isWeak(self.player) or self:getOverflow() >0) then
		need_useshende=true
	end
	if need_useshende then
		local ids=self.player:getPile("shende")
		ids = sgs.QList2Table(ids) 
		return sgs.Card_Parse(("peach:shende[%s:%s]=%d+%d"):format("to_be_decided", 0, ids[1], ids[2]))
	end
	return nil
end

sgs.ai_view_as.shende = function(card, player, card_place)
	if player:getMark("Global_PreventPeach")>0 then return false end
	local ids=player:getPile("shende")
	if ids:length()<2 then return false end
	ids = sgs.QList2Table(ids) 
	return ("peach:shende[%s:%s]=%d+%d"):format("to_be_decided", 0, ids[1], ids[2])
end
sgs.ai_cardneed.shende = function(to, card, self)
	return  card:isKindOf("Slash")
end
sgs.shende_keep_value = {
	Slash 			= 5.7
}




local gongfengvs_skill = {}
gongfengvs_skill.name = "gongfengvs"
table.insert(sgs.ai_skills, gongfengvs_skill)
function gongfengvs_skill.getTurnUseCard(self)
		if self.player:isKongcheng() then return nil end
        if self.player:getKingdom() ~="fsl" then return nil end
		if self.player:hasFlag("Forbidgongfeng") then return nil end
        local handcards = self:getCards("Slash")
        if #handcards  ==0 then return nil end
		self:sortByUseValue(handcards)

        return sgs.Card_Parse("@gongfengCard=" .. handcards[1]:getEffectiveId())
end
sgs.ai_skill_use_func.gongfengCard = function(card, use, self)
    local targets = {}
	for _,friend in ipairs(self.friends_noself) do
		if friend:hasSkill("gongfeng") then
			if not friend:hasFlag("gongfengInvoked") then
				table.insert(targets, friend)
			end
		end
	end
	if #targets > 0 then
		use.card = card
		if use.to then
			use.to:append(targets[1])
			return
		end
	end
end
sgs.ai_card_intention.gongfengCard = -40


function findBushuCard(self,from)
	local selfPoint = self:getMaxCard():getNumber()
	local fromCard 
	local fromPoint
	if self:isFriend(from) then
		fromCard = self:getMinCard(from)
	else
		fromCard = self:getMaxCard(from)
	end	
		if not fromCard then
			fromPoint = 6 
		else	
			fromPoint = fromCard:getNumber()
		end
	if selfPoint > fromPoint then
		return self:getMaxCard()
	else
		return self:getMinCard()
	end
end
sgs.ai_skill_invoke.bushu =function(self,data)
	local damage=self.player:getTag("bushu_damage"):toDamage()
	local from=damage.from
	local to=damage.to
	if self:isFriend(from) then return false end
	local need_pindian
	if self:isFriend(to) then
		if self:isWeak(to) then
			if not self.player:isKongcheng() then
				need_pindian =true
			end
		else
			if self.player:getHandcardNum()>1 then
				need_pindian = true
			end
		end
	end
	if need_pindian then
		bushucard = findBushuCard(self,from)
		self.bushu_card= bushucard:getEffectiveId()
		return true
	end
	return false
end
function sgs.ai_skill_pindian.bushu(minusecard, self, requestor, maxcard)
	if self:isFriend(requestor) then
		return self:getMinCard()
	end
	return self:getMaxCard()
end
sgs.ai_choicemade_filter.skillInvoke.bushu = function(self, player, promptlist)
	local damage = player:getTag("bushu_damage"):toDamage()
	local to =damage.to
	local from =damage.from

	if from and to and from:objectName()~= to:objectName() then
		if promptlist[#promptlist] == "yes" then
				sgs.updateIntention(player, to, -50)
				--sgs.updateIntention(player, from, 10)
		elseif sgs.evaluatePlayerRole(to) ~= "neutral" then
			local num = player:getHandcardNum()
			--if  num >= 3 and not self:isFriend(from, to)  then  --不用夹带私有的判断身份信息的isFriend  isFriend只适合用于自己的出牌策略 
			--update身份时，作为参考信息的身份信息 应该是公用列表的self.role的信息
			if num >= 3 and	sgs.evaluatePlayerRole(from) ~= sgs.evaluatePlayerRole(to) then
				sgs.updateIntention(player, to, 20)
			end
		end
	end
end

sgs.ai_cardneed.bushu = function(to, card, self)
	if not self:willSkipPlayPhase(to) then
		return  (not to:getWeapon() and  getCardsNum("Weapon",to,self.player)<1 and card:isKindOf("Weapon"))
		or (not to:getOffensiveHorse() and  getCardsNum("OffensiveHorse",to,self.player)<1 and card:isKindOf("OffensiveHorse"))
	end
end


sgs.ai_skill_playerchosen.chuancheng = function(self,targets)
	local target_table =sgs.QList2Table(targets)
	if #target_table==0 then return false end
	local chuancheng_target
	local lord
	if self.player:getRole() == "loyalist"  then
		lord=getLord(self.player)
	end
	if lord and not lord:hasSkill("chuancheng") then return lord end 
	self:sort(target_table, "value",true)
	for _,target in pairs(target_table) do 
		if  self:isFriend(target) then
			chuancheng_target=target
			break
		end
	end
	if chuancheng_target then
		return chuancheng_target
	end
	return nil
end


sgs.ai_skill_invoke.zhunbei  =function(self,data)
	if self.player:isKongcheng() then return false end
	return true
end

local qiji_skill = {}
qiji_skill.name = "qiji"
table.insert(sgs.ai_skills, qiji_skill)
qiji_skill.getTurnUseCard = function(self)
	if self.player:getHandcardNum()~=1 or self.player:getMark("@qiji")>0 then
		return nil
	end
	local card = self.player:getHandcards():first()
	if card:isKindOf("peach") then
		for _,p in pairs(self.friends) do
			if self:isWeak(p) then
				return nil
			end
		end
	end
	local choices={}
	
	local qiji = "peach|savage_assault|archery_attack|ex_nihilo|god_salvation|dismantlement"
	local qijis = qiji:split("|")
	for i = 1, #qijis do
		local forbiden = qijis[i]
		forbid = sgs.cloneCard(forbiden, card:getSuit(),card:getNumber())
		if self.player:isCardLimited(forbid, sgs.Card_MethodUse, true) or not forbid:isAvailable(self.player) then
		else
			table.insert(choices,qijis[i])
		end
	end
	local suit = card:getSuitString()
    local number = card:getNumberString()
    local card_id = card:getEffectiveId()
	
	local choice
	if  table.contains(choices,"peach") then
		choice="peach"
	end
	if not choice and table.contains(choices,"dismantlement") then
		for _,p in pairs(self.friends_noself) do
			if p:containsTrick("indulgence") or  p:containsTrick("supply_shortage")  then 
				choice="dismantlement"
			end
		end
	end
	if not choice and table.contains(choices,"god_salvation") then
		local aoe = sgs.cloneCard("god_salvation",card:getSuit(), card:getNumber())         
		if self:willUseGodSalvation(aoe) then
				choice="god_salvation"
		end
	end
	if not choice and table.contains(choices,"savage_assault") then
		local aoe = sgs.cloneCard("savage_assault",card:getSuit(), card:getNumber())        
		if self:getAoeValue(aoe, self.player)>0 then
				choice="savage_assault"
		end
	end
	if not choice and table.contains(choices,"archery_attack") then
		local aoe = sgs.cloneCard("archery_attack",card:getSuit(), card:getNumber())
		if self:getAoeValue(aoe, self.player)>0 then
				choice="archery_attack"
		end
	end
	if not choice and table.contains(choices,"ex_nihilo")  then
		choice="ex_nihilo"
	end
	if not choice then
		return nil
	end
	local str= (choice..":qiji[%s:%s]=%d"):format(suit, number, card_id)
                
	local parsed_card = sgs.Card_Parse(str)
	
	return parsed_card
end

function sgs.ai_cardsview_valuable.qiji(self, class_name, player)
	if player:getHandcardNum()~=1 or player:getMark("@qiji")>0 then
		return nil
	end
	local acard
	for _,c in sgs.qlist(player:getCards("h"))do
				acard=c
	end
	local suit =acard:getSuitString()
	local number = acard:getNumberString()
    local card_id = acard:getEffectiveId()
	
	if class_name == "Peach" then
		local dying = player:getRoom():getCurrentDyingPlayer()
		if not dying  then return nil end
		
		return ("peach:qiji[%s:%s]=%d"):format(suit, number, card_id) 
	end
	if class_name == "Jink" then
		return ("jink:qiji[%s:%s]=%d"):format(suit, number, card_id) 
	end
	if class_name == "Slash" then
		return ("slash:qiji[%s:%s]=%d"):format(suit, number, card_id) 
	end
	if class_name == "Nullification" then
		return ("nullification:qiji[%s:%s]=%d"):format(suit, number, card_id) 
	end
end


local fengshen_skill = {}
fengshen_skill.name = "fengshen"
table.insert(sgs.ai_skills, fengshen_skill)
function fengshen_skill.getTurnUseCard(self)
    if self.player:hasUsed("fengshenCard") then return nil end
	if self.player:isKongcheng() then return nil end
	local rcards = {}
	for _,c in sgs.qlist(self.player:getHandcards()) do
		if c:isRed() then
			table.insert(rcards,c)
		end
	end
	if #rcards==0 then return nil end
	self:sortByKeepValue(rcards)
	return sgs.Card_Parse("@fengshenCard=" .. rcards[1]:getEffectiveId())
end
sgs.ai_skill_use_func.fengshenCard = function(card, use, self)
        self:sort(self.enemies,"handcard")
        local targetsInAttackRange={}
		local targetsIn1={}
		local weaks={}
		local weaksIn1={}
		for _, p in ipairs(self.enemies) do
			if self.player:inMyAttackRange(p)  then    
				if self:getDamagedEffects(p, self.player) then
					continue
				end
				local fakeDamage=sgs.DamageStruct()
				fakeDamage.card=nil
				fakeDamage.nature= sgs.DamageStruct_Normal
				fakeDamage.damage=1
				fakeDamage.from=self.player
				fakeDamage.to=p
				local fengshen_effect= self:touhouNeedAvoidAttack(fakeDamage,self.player, p)
				if not  fengshen_effect then continue end
				table.insert(targetsInAttackRange,p)
				
				if self.player:distanceTo(p) == 1 then
					table.insert(targetsIn1,p)
				end
				if self:isWeak(p) then
					table.insert(weaks,p)
					if self.player:distanceTo(p) == 1 then
						table.insert(weaksIn1,p)
					end
				end
			end
        end
		--for _, p in ipairs(self.friends_noself) do
			--if self:getDamagedEffects(p, self.player) then
		--end
		if #weaks>0 and #weaksIn1 ==0 then
			use.card = card
			if use.to then
				use.to:append(weaks[1])
				return
			end
		end
		if #targetsIn1>0 then
			use.card = card
			if use.to then
				for _,p in ipairs(targetsIn1) do
					use.to:append(p)
				end
				return
			end
		end
		if #targetsInAttackRange>0 then
			use.card = card
			if use.to then
				use.to:append(targetsInAttackRange[1])
				return
			end
		end
end
sgs.ai_cardneed.fengshen = function(to, card, self)
	if not self:willSkipPlayPhase(to) then
		return  card:isRed()
	end
end
sgs.fengshen_suit_value = {
	heart=4,
	diamond = 4
}
sgs.ai_card_intention.fengshenCard = 50
--use_priority can not be a function ???? 
sgs.ai_use_priority.fengshenCard = sgs.ai_use_priority.Slash+0.2
sgs.dynamic_value.damage_card.fengshenCard = true

sgs.ai_skill_cardask["@fengshen-discard"] = function(self, data, pattern, target)
	local fakeDamage=sgs.DamageStruct()
	fakeDamage.card=nil
	fakeDamage.nature= sgs.DamageStruct_Normal 
	fakeDamage.damage=1
	fakeDamage.from=target
	fakeDamage.to=self.player
	if not self:touhouNeedAvoidAttack(fakeDamage,target,self.player) then
		return "." 
	end
	if self:getDamagedEffects(self.player, target) then
		return "."
	end
	local to_discard= self:getCards("Slash")
    if #to_discard  ==0 then return nil end
	self:sortByUseValue(to_discard)
	if #to_discard>0 then
		return "$" .. to_discard[1]:getId()
	end
end
sgs.ai_need_damaged.fengshen = function(self, attacker, player)
	if player:isWounded() then return false end
	if self:willSkipPlayPhase(player) then return false end
	if attacker and self:isFriend(attacker, player)  then return false end
	if not player:hasSkill("fengsu") then return false end
	local star = self.room:findPlayerBySkillName("ganying")
	if star and not self:isFriend(star, player) then
		return false
	end
	for _,p in sgs.qlist(self.room:getOtherPlayers(player)) do
		if player:distanceTo(p) == 2 and self:isEnemy(p,player) then
			return true
		end
	end
	return false
end





local xinshang_skill = {}
xinshang_skill.name = "xinshang"
table.insert(sgs.ai_skills, xinshang_skill)
function xinshang_skill.getTurnUseCard(self)
    if self.player:hasUsed("xinshangCard") then return nil end
	return sgs.Card_Parse("@xinshangCard=.")
end
sgs.ai_skill_use_func.xinshangCard = function(card, use, self)
        self:sort(self.enemies,"handcard")
        targets={}
		for _, p in ipairs(self.enemies) do
            if not p:isKongcheng() then    
				if self:touhouHandCardsFix(p) and p:getCards("e"):length()==0 then
					continue
				end
				table.insert(targets,p)
			end
        end
		if #targets >0 then
			use.card = card
            if use.to then
				use.to:append(targets[1])
				if use.to:length() >= 1 then return end
			end
        end
end
sgs.ai_skill_cardask["@xinshang-spadecard"] = function(self,data)
        local target=data:toPlayer()
		if not self:isFriend(target) and target:getHandcardNum()>=2 then return "." end
		local cards ={}
		for _,card in sgs.qlist(self.player:getCards("h")) do
			if card:getSuit()==sgs.Card_Spade then
				table.insert(cards,card)
			end
		end
		if #cards==0 then return "." end
		self:sortByKeepValue(cards)
        return "$" .. cards[1]:getId()
end
sgs.ai_use_value.xinshangCard = sgs.ai_use_value.Dismantlement + 1
sgs.ai_use_priority.xinshangCard = sgs.ai_use_priority.Dismantlement + 1
sgs.ai_card_intention.xinshangCard = 50
 

sgs.ai_damageInflicted.micai =function(self, damage)
	if  damage.to:getHandcardNum()<damage.damage then 
		damage.damage= damage.to:getHandcardNum()
	end
	return damage
end
	


local zaihuo_skill = {}
zaihuo_skill.name = "zaihuo"
table.insert(sgs.ai_skills, zaihuo_skill)
function zaihuo_skill.getTurnUseCard(self)
    if self.player:hasUsed("zaihuoCard") then return nil end
	if self.player:isKongcheng() then return nil end 
	if self:getOverflow() <=0 then return nil end 
	local handcards = sgs.QList2Table(self.player:getHandcards())
	
	self:sortByKeepValue(handcards)
	local spades={}
	local hearts={}
	local clubs={}
	local diamonds={}
	for _,c in pairs(handcards)do
		if c:getSuit()==sgs.Card_Spade then
			table.insert(spades,c)
		elseif c:getSuit()==sgs.Card_Heart then
			table.insert(hearts,c)
		elseif c:getSuit()==sgs.Card_Club then
			table.insert(clubs,c)
		else
			table.insert(diamonds,c)
		end
	end
    
	
	local others= sgs.QList2Table(self.room:getOtherPlayers(self.player))
	self:sort(others,"hp")
	local target
	local zaihuoCard
	for _,p in pairs(others) do
		if self:isFriend(p) then
			if p:hasSkills("chunxi|xingyun") then
				if #hearts>0 then
					zaihuoCard=hearts[1]
					target=p
					break
				end
			elseif p:isKongcheng() and not self:needKongcheng(p, true) then
					zaihuoCard=handcards[#handcards]
					target=p
					break
			else--辅助卖血流
				--ai_need_damaged此处的self为灾祸本人 
				--而一般的ai_need_damaged函数默认为技能人本人直接用了self.player，而不是player
				--所以需要各个具体的ai_need_damaged修改前 这条思路用不上。。。
				--local needDamaged=false
				--for _, skill in sgs.qlist(self.player:getVisibleSkillList()) do
				--	local callback = sgs.ai_need_damaged[skill:objectName()]
				--	if type(callback) == "function" and callback(self, nil, self.player) then
				--		needDamaged = true
				--		break
				--end
			end
		end
		if self:isEnemy(p) and not p:hasSkills("chunxi|xingyun") then
			local fakeDamage=sgs.DamageStruct()
			fakeDamage.card=nil
			fakeDamage.nature= sgs.DamageStruct_Normal
			fakeDamage.damage=1
			fakeDamage.from=nil
			fakeDamage.to=p
			local zaihuo_effect= self:touhouDamage(fakeDamage,nil,p).damage>0
			if not  zaihuo_effect then continue end
			for _,c in pairs(handcards) do
				if c:isKindOf("Peach") or c:isKindOf("Analeptic") then
				
				else
					
					zaihuoCard=c
					target=p
					break
				end
			end
		end
	end
		--[[if self:isWeak(p) then
			local spade=0
			local heart=0
			local club=0
			local diamond=0
			for _, c in sgs.qlist(p:getHandcards()) do
				local flag = string.format("%s_%s_%s", "visible", global_room:getCurrent():objectName(), p:objectName())
				if  c:hasFlag("visible") or c:hasFlag(flag) then
					if c:getSuit()==sgs.Card_Spade then
						spade=spade+1
					elseif c:getSuit()==sgs.Card_Heart then
						heart=heart+1
					elseif c:getSuit()==sgs.Card_Club then
						club=club+1
					elseif c:getSuit()==sgs.Card_Diamond then
						diamond=diamond+1
					end
				end
			end
		end]]
	
	
	if not target then return nil end
	local ai_data=sgs.QVariant()
	ai_data:setValue(target)
	self.player:setTag("zaihuo_target",ai_data)
	if target and  zaihuoCard then	
		return sgs.Card_Parse("@zaihuoCard=" .. zaihuoCard:getEffectiveId())
	end
end
sgs.ai_skill_use_func.zaihuoCard = function(card, use, self)
	local target = self.player:getTag("zaihuo_target"):toPlayer()
	if target then
		use.card=card
		if use.to then
			use.to:append(target)
			if use.to:length()>=1 then return end
		end
	end
end


sgs.ai_skill_invoke.jie = function(self,data)
	local damage = self.player:getTag("jie_damage"):toDamage()
	if not self:isFriend(damage.to) then
		return false
	end
	local nature = self:touhouDamageNature(damage.card, damage.from, self.player)
	if nature  ~= sgs.DamageStruct_Normal  then
		if self.player:isChained() and damage.to:isChained()  then
			return false
		end
	end

	
	local fakeDamage =  sgs.DamageStruct(damage.card, damage.from, self.player, damage.damage, nature)
	fakeDamage.transfer = true
	if  not self:touhouNeedAvoidAttack(fakeDamage, damage.from,self.player) then
		return true
	end
	
	local n1 = self:touhouDamage(damage,damage.from, damage.to).damage 
	local n2 =  self:touhouDamage(fakeDamage,damage.from, self.player).damage
	local damageReduce = n1 - n2 
	if damageReduce > 0 then
		return true
	elseif damageReduce < 0 then
		return false
	else
		return (self:isWeak(damage.to) and not self:isWeak(self.player))
			or not self.player:isWounded() 
	end
 	return false
end
sgs.ai_choicemade_filter.skillInvoke.jie = function(self, player, promptlist)
	local damage = player:getTag("jie_damage"):toDamage()
	local to=damage.to
	if  to  then
		if promptlist[#promptlist] == "yes" then
			sgs.updateIntention(player, to, -50)
		end	
	end
end
--[[sgs.ai_cardneed.jie = function(to, card, self)
	if not self:willSkipPlayPhase(to) then
		return  (not to:getWeapon() and  getCardsNum("Weapon",to,self.player)<1 and card:isKindOf("Weapon"))
		or (not to:getOffensiveHorse() and  getCardsNum("OffensiveHorse",to,self.player)<1 and card:isKindOf("OffensiveHorse"))
	end
end]]
function SmartAI:canJie(player)
	for _,p in sgs.qlist(self.room:getOtherPlayers(player)) do
		if self:isFriend(p, player) and p:hasSkill("jie") and p:getHp() > 1  then
			return true
		end
	end
	return false 
end



sgs.ai_skill_cardask["@liuxing"] = function(self,data)
    local blacks ={}
	for _, c in sgs.qlist(self.player:getCards("h")) do
		if c:isBlack() then
			table.insert(blacks,c)
		end
	end
	if #blacks==0 then return "." end
	local current = self.room:getCurrent()
	if self:isEnemy(current) and self:touhouHpLocked(current) then
		return "."
	end
	self:sortByUseValue(blacks)
    return "$" .. blacks[1]:getId()
end

sgs.ai_skill_choice.liuxing = function(self, choices, data)	
	local hina = data:toPlayer()
	if self:isFriend(hina) then return "recover" end
	if self:touhouHpLocked(self.player) then 
		return "losehp"
	end
	return "recover"
end
sgs.ai_choicemade_filter.skillChoice.liuxing = function(self, player, promptlist)
	local hina  = player:getTag("liuxing_source"):toPlayer()
	if hina then
		if promptlist[#promptlist] == "losehp" then
			sgs.updateIntention(player, hina, 50)
		end	
	end
end

function SmartAI:cautionChangshi()
	local sanae = self.room:findPlayerBySkillName("changshi")
	if sanae then return true end
	local yukari = self.room:findPlayerBySkillName("xijian")
	if yukari and not self:isFriend(yukari) then return true end
	return false
end


sgs.ai_skill_invoke.jinian = true

local tianyan_skill = {}
tianyan_skill.name = "tianyan"
table.insert(sgs.ai_skills, tianyan_skill)
function tianyan_skill.getTurnUseCard(self)
    if self.player:hasUsed("tianyanCard") then return nil end
	return sgs.Card_Parse("@tianyanCard=.")
end
sgs.ai_skill_use_func.tianyanCard = function(card, use, self)
	use.card=card
end
sgs.ai_use_value.tianyanCard = 8
sgs.ai_use_priority.tianyanCard = 7



local fengrang_skill = {}
fengrang_skill.name = "fengrang"
table.insert(sgs.ai_skills, fengrang_skill)
function fengrang_skill.getTurnUseCard(self)
	if self.player:hasUsed("fengrangCard") then return nil end
	local dummy_use = { isDummy = true, to = sgs.SPlayerList() }
	local card=sgs.cloneCard("amazing_grace", sgs.Card_NoSuit, 0)
	card:setSkillName("fengrang")
	self:useTrickCard(card, dummy_use)
	if not dummy_use.card then return nil end
	return sgs.Card_Parse("@fengrangCard=.")
end
sgs.ai_skill_use_func.fengrangCard = function(card, use, self)
	use.card=card
end
sgs.ai_use_priority.fengrangCard = sgs.ai_use_priority.AmazingGrace
sgs.ai_use_value.fengrangCard = 3
sgs.dynamic_value.benefit.fengrangCard = true


 


function jiliaoParse(self, player)
	local effect =false
	if (self:hasSkills(sgs.lose_equip_skill, player) and  player:getCards("e"):length()>0)
	or self:needToThrowArmor(player) then
		effect =true 
	end
	--[[if player:hasSkill("mokai") then
		local marks={"@tianyi_Weapon","@tianyi_OffensiveHorse","@tianyi_DefensiveHorse","@tianyi_Armor"}
		for _,m in pairs(marks)do
			if player:getMark(m)>0 then
				effect =true
				break
			end
		end
	end]]
	local ecards=player:getCards("e")
	if  ecards:length()>0 and player:hasSkills("chunxi|xingyun") then    
		for _,c in sgs.qlist(ecards)do
			if c:getSuit()==sgs.Card_Heart then
				effect=true
				break
			end
		end	
	end
	local candiscard = ecards:length()+player:getHandcardNum()>player:getMaxCards() 
	return effect,candiscard
end
local jiliao_skill = {}
jiliao_skill.name = "jiliao"
table.insert(sgs.ai_skills, jiliao_skill)
function jiliao_skill.getTurnUseCard(self)
	if self.player:hasUsed("jiliaoCard") then return nil end
	return sgs.Card_Parse("@jiliaoCard=.")
end
sgs.ai_skill_use_func.jiliaoCard = function(card, use, self)
	self:sort(self.enemies,"handcard",true)
    local targets={}
	local tmp_targets={}
	for _, p in ipairs(self.enemies) do
		local prohibit, overflow = jiliaoParse(self, p)
		if overflow then
			if not prohibit then
				table.insert(targets,p)
			end
		else
			if p:getCards("e"):length()>0 and not prohibit then
				if self:isWeak(p) and (p:getArmor() or p:getDefensiveHorse()) then
					table.insert(targets,p)
				else
					table.insert(tmp_targets,p)
				end
			end
		end	
    end
	for _, p in ipairs(self.friends_noself) do
		local effect = jiliaoParse(self, p)
		if effect then
			table.insert(targets,p)
		end
	end
	if #targets >0 then
		self:sort(targets,"value",true)
		use.card = card
        if use.to then
			use.to:append(targets[1])
			if use.to:length() >= 1 then return end
		end
	else
		if #tmp_targets >0 then
			self:sort(tmp_targets,"value",true)
			use.card = card
			if use.to then
				use.to:append(tmp_targets[1])
				if use.to:length() >= 1 then return end
			end
		end
    end
end
sgs.ai_skill_invoke.jiliao = function(self,data)
	local strs = data:toStringList()
	local name= (strs[1]:split(":"))[2]
	local target 
	for _,p in sgs.qlist(self.room:getOtherPlayers(self.player)) do
		if p:objectName() == name then
			target=p
			break
		end
	end
	if target and self:isEnemy(target) then
		return true
	end
	return false
end
sgs.ai_use_value.jiliaoCard = sgs.ai_use_value.Dismantlement + 1
sgs.ai_use_priority.jiliaoCard = sgs.ai_use_priority.Dismantlement + 1
sgs.ai_card_intention.jiliaoCard = function(self, card, from, tos)
	local effect  = jiliaoParse(self, tos[1])
	if not effect then
		sgs.updateIntentions(from, tos, 20)
	end
end

sgs.ai_skill_invoke.zhongyan = function(self,data)
	local damage = self.room:getTag("zhongyan_damage"):toDamage()
	local target=damage.from
	local t=false
	
	local hp_after_damage=self.player:getHp()-damage.damage
	if  hp_after_damage<=0 then
		t=true
	end
	if self:isEnemy(target) then
		if t then 
			return true
		end
		if self:touhouHpLocked(target) then
			return false 
		elseif (target:getLostHp()>=2) then
			return true
		end
	end
	return false
end
sgs.ai_choicemade_filter.skillInvoke.zhongyan = function(self, player, promptlist)
	local from=self.room:getTag("zhongyan_damage"):toDamage().from
	
	if from then
		if promptlist[#promptlist] == "yes" and from:getLostHp()>0 then
			sgs.updateIntention(player, from, 60)
		end
	end
end
sgs.ai_need_damaged.zhongyan = function(self, attacker, player)
	if player:getMark("@zhongyan") == 0  then return false end
	if not self:getDamageSource(attacker) then return false end
	if self:touhouHpLocked(attacker) then
		return false
	end

	if attacker:hasSkill("bumie") then return false end
	if  self:isEnemy(attacker,player) then
		if attacker:getLostHp()>= attacker:getHp()then
			return true
		end
	end
	return false
end
sgs.ai_slash_prohibit.zhongyan = function(self, from, to, card)
	if self:isFriend(from,to) then
		return false
	end
	if to:getMark("@zhongyan")==0 then return false end
	local callback=sgs.ai_damage_prohibit["zhongyan"]
	return callback(self, from, to, card)
end
sgs.ai_trick_prohibit.zhongyan = function(self, from, to, card)
	if self:isFriend(from,to) then
		return false
	end
	if to:getMark("@zhongyan")==0 then return false end
	if not self:touhouIsDamageCard(card) then return false end
	local callback=sgs.ai_damage_prohibit["zhongyan"]
	return callback(self, from, to, card)
end
sgs.ai_damage_prohibit.zhongyan = function(self, from, to, card)
	if self:isFriend(from,to) then
		return false
	end
	if to:getMark("@zhongyan")==0 then return false end
	if self:touhouHpLocked(from) then
		return false
	end
	if (from:getLostHp()>=2 or from:getHp()<=1)  then
		local fakeDamage=sgs.DamageStruct()
		if card then
			fakeDamage.card=card
		end
		fakeDamage.nature= sgs.DamageStruct_Normal 
		fakeDamage.damage=1
		fakeDamage.from=from
		fakeDamage.to=to
		if not self:touhouDamageEffect(fakeDamage,from,to) then 
			return true
		end
	end
	return false
end

local dfgzmsiyu_skill = {}
dfgzmsiyu_skill.name = "dfgzmsiyu"
table.insert(sgs.ai_skills, dfgzmsiyu_skill)
function dfgzmsiyu_skill.getTurnUseCard(self)
	if self.player:hasUsed("dfgzmsiyuCard") then return nil end
	local cards=self.player:getCards("h")
	
	local can_qishu=true
	if #self.enemies<2 then 
		can_qishu=false
	end	
	if not self.player:hasSkill("qishu") then
		can_qishu=false
	end
	local qishu_card
	if can_qishu then
		for _,c in sgs.qlist(cards) do
			if c:isKindOf("Jink") then
				qishu_card=c
				break
			end
		end
	end
	if can_qishu and cards:length()==2 and not qishu_card then
		if cards:first():isKindOf("Dismantlement") or  cards:first():isKindOf("Snatch") then
			qishu_card=cards:last()
		elseif cards:last():isKindOf("Dismantlement") or  cards:last():isKindOf("Snatch") then
			qishu_card=cards:first()
		elseif cards:first():isKindOf("Slash") and sgs.Slash_IsAvailable(self.player) then
			qishu_card=cards:last()
		elseif cards:last():isKindOf("Slash") and sgs.Slash_IsAvailable(self.player) then
			qishu_card=cards:first()
		end
	end
	sgs.ai_use_priority.dfgzmsiyuCard = (qishu_card and can_qishu) and 8 or 0
	if not qishu_card then
		cards = sgs.QList2Table(cards)
		self:sortByKeepValue(cards)
		qishu_card=cards[1]
	end
	if (qishu_card) then 
		return sgs.Card_Parse("@dfgzmsiyuCard=" .. qishu_card:getEffectiveId())
	end
	return nil
end
sgs.ai_skill_use_func.dfgzmsiyuCard = function(card, use, self)
	local target
	self:sort(self.enemies,"handcard")
	for var=#self.enemies, 1, -1 do
		if self.enemies[var]:hasSkills("chunxi|xingyun") then
			local heart=sgs.Sanguosha:getCard(card:getSubcards():first())
			if heart:getSuit()~=sgs.Card_Heart then
				target=self.enemies[var]
				break
			end
		else
			target=self.enemies[var]
			break
		end
	end
	if target then
		use.card=card
		if use.to then
			use.to:append(target)
			if use.to:length()>=1 then return end
		end
	end
end
sgs.ai_skill_cardchosen.dfgzmsiyu = function(self, who, flags)
	local cards= sgs.QList2Table(who:getCards("h"))
	local inverse = not self:isFriend(who)
	self:sortByKeepValue(cards, inverse)
	return cards[1]
end


