--ԭ����ɱͨ���޸�
--������
--����������˳�ֱ���и�ɻ�
--sgs.ai_choicemade_filter.Nullification.general

--����ɱ����ͨ�ú���
--SmartAI:touhouDamageNature
--SmartAI:touhouDamage
--SmartAI:touhouDamageCaused
--SmartAI:touhouDamageInflicted
--SmartAI:touhouDamageEffect
--SmartAI:touhouEffectNullify
--SmartAI:touhouCardEffectNullify
--SmartAI:touhouDamageBenefit
--SmartAI:touhouHpValue
--SmartAI:touhouChainDamage
--SmartAI:touhouIgnoreArmor
--SmartAI:touhouCanHuanshi
--SmartAI:touhouSidieTarget
--SmartAI:touhouHandCardsFix
--SmartAI:touhouDrawCardsInNotActive
--SmartAI:touhouGetAoeValueTo
--SmartAI:touhouDelayTrickBadTarget
--SmartAI:touhouHpLocked
--SmartAI:touhouIsPriorUseOtherCard
--SmartAI:touhouDummyUse


--���ַ���
--sgs.no_intention_damage
--sgs.intention_damage
--sgs.intention_recover


--sgs.ai_1v1 

--��������ai
--SmartAI:needRetrial(judge) ������������
function SmartAI:lingqiParse(self,target)
	--1���� 2 �к�
	local use=self.player:getTag("lingqicarduse"):toCardUse()
	if not use or not use.card  or not use.from then return 3 end
	local card=use.card
	local from=use.from
	
	if (card:isKindOf("Slash") or card:isKindOf("ArcheryAttack")) and target:hasSkill("hongbai") and self:hasEightDiagramEffect(target) then
		return 3
	end
	if card:isKindOf("Slash") or card:isKindOf("SavageAssault")  
	or card:isKindOf("ArcheryAttack")  or card:isKindOf("Duel") 
	or card:isKindOf("FireAttack") then
		return 2
	end
	--�����ʱ���ܡ�����
	if card:isKindOf("ExNihilo") then
		return 1
	end
	if card:isKindOf("GodSalvation") and target:isWounded() then
		return 1
	end
	if card:isKindOf("IronChain")  then
		if target:isChained() then
			return 1
		else
			return 2
		end
	end
	if card:isKindOf("Snatch") or card:isKindOf("Dismantlement") then
		local cards=target:getCards("j")
		--û��������
		local throw_delay=false
		for _,card in sgs.qlist(cards) do
			if card:isKindOf("Indulgence") or card:isKindOf("SupplyShortage") then
				throw_delay=true
			end
		end
		if  throw_delay and self:isFriend(from,target) then
			return 1
		else
			return 2
		end
	end
	return 3
end
function must_lingqi(self)--must��Ҫ�����Ա�
	local use=self.player:getTag("lingqicarduse"):toCardUse()
	local must_lingqi=false
	if use.card:isKindOf("Slash") and not self:hasEightDiagramEffect(self.player) and use.to:contains(self.player) then
		if self:getCardsNum("Jink")==0 then
			must_lingqi=true
		end
	end
	if use.card:isKindOf("Snatch") and 
	not (self:isFriend(use.from) and self.player:containsTrick("supply_shortage"))  then
		must_lingqi=true
	end
	return must_lingqi
end
function lingqi_discard_parse(self,cards,must_lingqi)
	local to_discard = {}
	self:sortByKeepValue(cards) 
	for _,card in pairs(cards) do
		--�����ر�ؼ�ʱ�� �ң����� ����Ҫ����
		if not (card:isKindOf("Peach") or card:isKindOf("Vine") or card:isKindOf("EightDiagram")) then
			table.insert(to_discard, card:getId())
			break
		end
	end
	if #to_discard>0 then
		return to_discard
	else
		if must_lingqi then
			table.insert(to_discard, cards[1]:getId())
			return to_discard
		end
	end
	return to_discard
end
sgs.ai_skill_use["@@lingqi"] = function(self, prompt)
	if not self:invokeTouhouJudge() then return "." end
    local use=self.player:getTag("lingqicarduse"):toCardUse()
	local targets={}--����
	local targets1={}--����
	local targets2={}--�ѷ�
	local must_do=must_lingqi(self)
	num=self.player:getCards("he"):length()
	for _, p in sgs.qlist(use.to) do
		local parse=self:lingqiParse(self,p)
		if self:isFriend(p) and parse==2 then
			if self:isWeak(p) 
			or (p:objectName()==self.player:objectName() and must_do) then
				table.insert(targets,p:objectName())
				table.insert(targets2,p:objectName())			
			else
				table.insert(targets1,p:objectName())	
				table.insert(targets2,p:objectName())
			end
		end
		if self:isEnemy(p) and parse==1 then
			if self:isWeak(p) then
				table.insert(targets,p:objectName())
				table.insert(targets2,p:objectName())
			else
				table.insert(targets1,p:objectName())
				table.insert(targets2,p:objectName())
			end
		end
    end		 
	target_num= #targets2
	cards=self.player:getCards("he")
	cards = sgs.QList2Table(cards)
	local ids=lingqi_discard_parse(self,cards,must_do)--table
	
	if #ids==0 then  return "." end
	if #targets ==0 and #targets1 ==0 then return "."	 end
	
	if num>4 then
		return "#lingqi:".. ids[1] ..":->" .. table.concat(targets2, "+")
	else	
		if (#targets>0)  or (target_num>2)  or table.contains(targets,self.player:objectName()) then
			return "#lingqi:".. ids[1] ..":->" .. table.concat(targets2, "+")
		else
			return "."
		end
	end
	return "."
end
sgs.lingqi_keep_value = {
	Vine = 7, 
}

--����ס�ai 
--sgs.ai_armor_value.EightDiagram 
--function SmartAI:needRetrial(judge)
--function SmartAI:getRetrialCardId(cards, judge, self_card)
--function sgs.getDefense(player, gameProcess)
sgs.hongbai_keep_value = {
	EightDiagram = 8
}

--��������ai
--�ж��Ƿ���Ҫ����
sgs.ai_skill_invoke.boli = function(self,data) 
	local judge=self.player:getTag("boli_judge"):toJudge()
	return self:needRetrial(judge)--�������ܵ��ж�Ч����good bad���������������ھ��������good bad ���岻һ����������
end
sgs.ai_skill_cardask["@boli-retrial"] = function(self, data)
        local lord = getLord(self.player)
		if not self:isFriend(lord) then return "." end
		--�ֱ�����Ϊ ����Ҫ�������ж��Ƕ����������� ��Ȼ����
		local cards = sgs.QList2Table(self.player:getCards("h"))
		cards1={}
		for _,card in pairs(cards) do
			if card:getSuit()==sgs.Card_Heart then
				table.insert(cards1,card)
			end
		end
        local judge = data:toJudge()
        --judge.card = cards[1]
        --table.removeOne(cards, judge.card)
		if #cards1==0 then return "." end
        return "$" .. self:getRetrialCardId(cards1, judge) or judge.card:getEffectiveId()  --tostring()
end

--��ħ����ai  
--function SmartAI:hasHeavySlashDamage(from, slash, to, getValue)
local mofa_skill = {}
mofa_skill.name = "mofa"
table.insert(sgs.ai_skills, mofa_skill)
function mofa_skill.getTurnUseCard(self)
    if self.player:hasFlag("mofa_invoked") then return nil end
	local cards = self.player:getCards("h")
	if #self.enemies==0 then return false end
	--����Ҫϸ�������ܷ����
	local key_names={"Slash","SavageAssault","ArcheryAttack"}
	local key_cards={}
	for _,name in pairs (key_names) do
		for _,c in sgs.qlist(cards) do
			if c:isKindOf(name) and  c:isAvailable(self.player) then
				table.insert(key_cards,c)
			end
		end
	end
	if #key_cards==0 then return false end
	local scards={}
	local scards_without_key={}
	for _,card in sgs.qlist(cards) do
		if	card:getSuit()==sgs.Card_Spade then
			table.insert(scards,card)
			if not table.contains(key_cards,card) then
				table.insert(scards_without_key,card)
			end
		end
	end
	if #scards_without_key>0 then 
		self:sortByCardNeed(scards_without_key)
		return sgs.Card_Parse("#mofa:" .. scards_without_key[1]:getEffectiveId() .. ":")
	else
		if #scards>0 and #key_cards>1 then--�к��ң�����ȫ�Ǻ���key
			self:sortByUseValue(scards)
			return sgs.Card_Parse("#mofa:" .. scards[1]:getEffectiveId() .. ":")
		end	
	end
	
	return false
end
sgs.ai_skill_use_func["#mofa"] = function(card, use, self)
	use.card=card
end
sgs.ai_use_value.mofa = 4
sgs.ai_use_priority.mofa = 4

--���ڰס�ai  ������ ����Ҫai  ��������ҪһЩ�Բ�

--�����꡿ai --�������ˣ�
local wuyuvs_skill = {}
wuyuvs_skill.name = "wuyuvs"
table.insert(sgs.ai_skills, wuyuvs_skill)
wuyuvs_skill.getTurnUseCard = function(self)
	if self.player:hasFlag("Forbidwuyu") then return nil end

	local cards = self.player:getCards("h")
	cards = sgs.QList2Table(cards)
	local card
	self:sortByUseValue(cards,true)
	for _,acard in ipairs(cards)  do
		if acard:getSuit()==sgs.Card_Spade then
			card = acard
			break
		end
	end
	if not card then return nil end
	
	local card_id = card:getEffectiveId()
	--local card_str ="#wuyuvs:.:"
	local card_str ="#wuyuvs:"..card_id..":"
	local skillcard = sgs.Card_Parse(card_str)
	
	assert(skillcard)
	
	return skillcard
end

sgs.ai_skill_use_func["#wuyuvs"] = function(card, use, self)
	
	local targets = {}
	for _,friend in ipairs(self.friends_noself) do
		if friend:hasLordSkill("wuyu") then
			if not friend:hasFlag("wuyuInvoked") then
				table.insert(targets, friend)
			end
		end
	end
	
	if #targets > 0 then --���꼺��
		use.card = card
		if use.to then
			use.to:append(targets[1])
			if use.to:length()>=1 then return end
		end
	end
	--return "."
	--���ƻ���������������÷����ݲ�����
end
sgs.ai_card_intention.wuyuvs = -40

--����Ǯ��ai
function SmartAI:hasSkillsForSaiqian(player)
	player = player or self.player
	if player:hasSkills("xisan|yongheng") then
		return true
	end
	if player:hasSkills("zaozu+qiuwen") then
		return true
	end
	return false
end
local saiqianvs_skill = {}
saiqianvs_skill.name = "saiqianvs"
table.insert(sgs.ai_skills, saiqianvs_skill)
function saiqianvs_skill.getTurnUseCard(self)
		if self.player:isKongcheng() then return nil end
        if self.player:hasFlag("ForbidSaiqian") then return nil end
        local handcards = sgs.QList2Table(self.player:getHandcards())
		local saiqian_cards={}
		self:sortByUseValue(handcards)
		if self.player:isWounded()then			
			for _,c in pairs(handcards)do
				if c:getSuit()==sgs.Card_Heart then
					table.insert(saiqian_cards,c:getId())
					break
				end
			end
		end
		
       if self:getOverflow() >0 or self:hasSkillsForSaiqian(self.player)  then
			local overflow=math.min(3,self:getOverflow())
			if self:hasSkillsForSaiqian() then
				overflow= math.min(3,self.player:getHandcardNum())
			end
			for var=1, overflow, 1 do
				if #saiqian_cards<self:getOverflow() 
					and not table.contains(saiqian_cards,handcards[var]:getId()) then
					table.insert(saiqian_cards,handcards[var]:getId())
				end
			end
	   end
		if #saiqian_cards>0 then
			return sgs.Card_Parse("#saiqianvs:" .. table.concat(saiqian_cards, "+") .. ":")	
		end
		return nil
end
sgs.ai_skill_use_func["#saiqianvs"] = function(card, use, self)
    local targets = {}
	for _,friend in ipairs(self.friends_noself) do
		if friend:hasSkill("saiqian") then
			if not friend:hasFlag("saiqianInvoked") then
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
sgs.ai_skill_choice.saiqian= function(self, choices, data)	
	local source=self.player:getTag("saiqian_source"):toPlayer()
	
	if not source or not source:isWounded() or not self:isFriend(source) then return "cancel_saiqian" end
	if choices:match("discard_saiqian") then
		for _,c in sgs.qlist(self.player:getHandcards())do
			if c:getSuit()==sgs.Card_Heart  then
				return "discard_saiqian"
			end
		end
	end
	if self.player:getHp()>3 and choices:match("losehp_saiqian") then
		return "losehp_saiqian"
	end
	return "cancel_saiqian"
end
sgs.ai_skill_cardask["@saiqian-discard"] = function(self,data)
        local cards = sgs.QList2Table(self.player:getCards("h"))
        if #cards==0 then return "." end
		self:sortByCardNeed(cards)
		for _,card in pairs (cards) do
			if card:getSuit()==sgs.Card_Heart then
				return "$" .. card:getId()
			end
		end
		return "."
end
sgs.ai_card_intention.saiqianvs = -60
--[[sgs.ai_choicemade_filter.skillChoice.saiqian = function(self, player, promptlist)
	local choice = promptlist[#promptlist]
	local target =player:getTag("saiqian_source"):toPlayer()
	
	if not target then return end
	if  choice == "losehp_saiqian" then 
		sgs.updateIntention(player, target, -10) 
	end
end]]
sgs.saiqian_suit_value = {
	heart = 4.9
}

--�����ߡ�ai
local jiezou_skill = {}
jiezou_skill.name = "jiezou"
table.insert(sgs.ai_skills, jiezou_skill)
jiezou_skill.getTurnUseCard = function(self)
	--û�����Լ��˵����.
	local targets=self:getEnemies(self.player)
	cards=self.player:getCards("h")
	can_use=false
	for _,card in sgs.qlist(cards) do
		if card:getSuit()==sgs.Card_Spade then
			can_use=true
			break
		end
	end
	for _,target in pairs(targets) do
		if can_use then
			break
		end
		ecards=target:getCards("e")
		for _,card in sgs.qlist(ecards) do
			if card:getSuit()==sgs.Card_Spade then
				can_use=true
				break
			end
		end
	end
	if can_use then
		return sgs.Card_Parse("#jiezou:.:")
	end
	return nil
end
sgs.ai_skill_use_func["#jiezou"] = function(card, use, self)
        
		local friends={}
		local enemies_e={}
		local enemies_h={}
		
		for _, p in sgs.qlist(self.room:getOtherPlayers(self.player)) do
            if p:isAllNude() then continue end
			if self:isFriend(p) then
				local jcards=p:getCards("j")
				for _,card in sgs.qlist(jcards) do
					if card:getSuit()==sgs.Card_Spade and not card:isKindOf("Lightning") then
						table.insert(friends,p)
					end
				end
			end
			if self:isEnemy(p) then
				local ecards=p:getCards("e")
				for _,card in sgs.qlist(ecards) do
					if card:getSuit()==sgs.Card_Spade then
						table.insert(enemies_e,p)
					end
				end
				if not p:isKongcheng() then
					table.insert(enemies_h,p)
				end
			end
        end
        if #friends>0 then
			use.card = card
            if use.to then
                self:sort(friends,"value")
				use.to:append(friends[1])
				if use.to:length() >= 1 then return end
            end
		elseif #enemies_e>0 then
			use.card = card
            if use.to then
                self:sort(enemies_e,"value")
				use.to:append(enemies_e[1])
				if use.to:length() >= 1 then return end
            end
		elseif #enemies_h>0 then
			use.card = card
            if use.to then
                self:sort(enemies_h,"value")
				use.to:append(enemies_h[1])
				if use.to:length() >= 1 then return end
            end
		end
end
sgs.ai_skill_cardchosen.jiezou = function(self, who, flags)
	if self:isFriend(who) then
		local jcards=who:getCards("j")
		for _,card in sgs.qlist(jcards) do
			if card:getSuit()==sgs.Card_Spade and not card:isKindOf("Lightning") then
				return card
			end
		end
	else
		local ecards=who:getCards("e")
		for _,card in sgs.qlist(ecards) do
			if card:getSuit()==sgs.Card_Spade then
				return card
			end
		end
		local hcards=who:getCards("h")
		return hcards:first()
	end
end
sgs.ai_skill_cardask["jiezou_spadecard"] = function(self, data)
        local cards = sgs.QList2Table(self.player:getCards("he"))
        if #cards==0 then return "." end
		self:sortByCardNeed(cards)
		for _,card in pairs (cards) do
			if card:getSuit()==sgs.Card_Spade then
				return "$" .. card:getId()
			end
		end
		return "."
end

sgs.ai_choicemade_filter.cardChosen.jiezou = sgs.ai_choicemade_filter.cardChosen.snatch
sgs.ai_use_value.jiezou = 8
sgs.ai_use_priority.jiezou =5
sgs.jiezou_suit_value = {
	spade = 4.9
}

--���ղء�ai 
function keycard_shoucang(card)
	if card:isKindOf("Peach") or card:isKindOf("SavageAssault")  
	or card:isKindOf("ArcheryAttack") or card:isKindOf("ExNihilo") then
		return true
	end
	return false
end
sgs.ai_skill_use["@@shoucang"] = function(self, prompt)
	if self:getOverflow() <= 0 then
		return "."
	end
	local needNum=math.min(4,self:getOverflow())
	--������ɫ�ֿ�
	local spades={}
	local hearts={}
	local clubs={}
	local diamonds={}
	
	for _,card in sgs.qlist(self.player:getCards("h")) do
		if card:getSuit()==sgs.Card_Spade then
			table.insert(spades,card)
		end
		if card:getSuit()==sgs.Card_Heart then
			table.insert(hearts,card)
		end
		if card:getSuit()==sgs.Card_Club then
			table.insert(clubs,card)
		end
		if card:getSuit()==sgs.Card_Diamond then
			table.insert(diamonds,card)
		end
	end
	
	shows={}
	s=true
	h=true
	c=true
	d=true
	showNum=0
	--˳��Ϊ÷�������飬����,���ң�
	if #clubs>0 and showNum<needNum then
		for _,card in pairs(clubs) do
			if not keycard_shoucang(card) then
				table.insert(shows,tostring(card:getId()))
				c=false
				showNum=showNum+1
				break
			end
		end
	end
	if #diamonds>0 and showNum<needNum then
		for _,card in pairs(diamonds) do
			if not keycard_shoucang(card) then
				table.insert(shows,tostring(card:getId()))
				c=false
				break
			end
		end
	end
	if #spades>0 and showNum<needNum then
		for _,card in pairs(spades) do
			if not keycard_shoucang(card) then
				table.insert(shows,tostring(card:getId()))
				s=false
				break
			end
		end
	end
	if #hearts>0 and showNum<needNum then
		for _,card in pairs(hearts) do
			if  keycard_shoucang(card) then
				table.insert(shows,tostring(card:getId()))
				h=false
				break
			end
		end
	end
	if showNum<needNum then --�ؼ���Ҳ��չʾ��
		--˳��Ϊ÷�������飬����,���ң�
		if #clubs>0 and showNum<needNum and c then
		for _,card in pairs(clubs) do
			if  keycard_shoucang(card) then
				table.insert(shows,tostring(card:getId()))
				c=false
				showNum=showNum+1
				break
			end
		end
		end
		if #diamonds>0 and showNum<needNum and d then
		for _,card in pairs(diamonds) do
			if  keycard_shoucang(card) then
				table.insert(shows,tostring(card:getId()))
				c=false
				break
			end
		end
		end
		if #spades>0 and showNum<needNum and s then
		for _,card in pairs(spades) do
			if  keycard_shoucang(card) then
				table.insert(shows,tostring(card:getId()))
				s=false
				break
			end
		end
		end
		if #hearts>0 and showNum<needNum and h then
		for _,card in pairs(hearts) do
			if  keycard_shoucang(card) then
				table.insert(shows,tostring(card:getId()))
				h=false
				break
			end
		end
		end
	end
	if #shows>0 then
		return "#shoucang:" ..table.concat(shows, "+")..":"
	end
	return "."
end	



--�����¡�ai
--useEquipCard
function SmartAI:needBaoyiEquip(card,player)
	player = player or self.player
	if not player:hasSkill("baoyi") then return false end
	t =false
	Overflow = self:getOverflow() <2
	if not card:isKindOf("EquipCard") then return false end
	--local equip = card:getRealCard():toEquipCard()
                
	--if  player:getEquip(equip:location()) and Overflow then 
	--	return true
	--end
	if self:getSameEquip(card) and Overflow then
		return true
	end
	return false
end
sgs.ai_skill_invoke.baoyi = function(self,data)
	--�������µ��ж�
	if self.player:getCards("j"):length() >0 then
		return true
	else
		target_table = self:getEnemies(self.player)
		if #target_table==0 then return false end
		cards=self.player:getCards("e")
		if cards:length()>=2 then
			return true
		else	
			local t =false
			for _,card in sgs.qlist(cards) do
				if card:getSuit()==sgs.Card_Spade then
					t=true
					break
				end
			end
			return t
		end
	end
end
sgs.ai_skill_invoke.baoyi_draw = true

sgs.ai_skill_playerchosen.baoyi = function(self, targets)
	local target_table = sgs.QList2Table(targets)
	self:sort(target_table, "hp")
	local baoyi_target
	for _,target in pairs(target_table) do 
		if  self:isEnemy(target) then
			if not baoyi_target  then
				baoyi_target=target
			else
				if self:isWeak(target) and not self:isWeak(baoyi_target) then
					baoyi_target=target
					break
				end
			end
		end
	end
	
	if baoyi_target then
		return baoyi_target
	end
	return nil
end
sgs.baoyi_suit_value = {
	spade = 4.9
}
sgs.ai_trick_prohibit.baoyi = function(self, from, to, card)
	if not card:isKindOf("DelayedTrick")  then return false end
	if self:isFriend(from,to) then return false end
	--Ŀǰ�Ե��˵ı�������ʱ����ֻ���ǵ���+����
	if self.room:alivePlayerCount()==2 then
		return not from:hasArmorEffect("Vine")
	else
		return true
	end
	return true
end
--��ְ��ai
--Ӧ�òο����� ����д�úܼ򵥡�����
sgs.ai_skill_playerchosen.aojiaofsuzhize = function(self, targets)
	--��һ���������� Ӧ�ò����к���
	local pre_zhize=self.player:getTag("pre_zhize"):toPlayer()
	local target_table = sgs.QList2Table(targets)
	local zhize_target
	local targets={}
	local badtargets={}
	local ftargets={}
	for _,target in pairs(target_table) do 
		if  self:isEnemy(target)  then
			if target:hasSkills("yongheng|kongpiao") then
				table.insert(badtargets,target)
			else
				table.insert(targets,target)
			end
		else
			if target:hasSkills("yongheng|kongpiao") then
				table.insert(ftargets,target)
			end
		end
	end
	if #targets==0 then
		for _,target in pairs(target_table) do 
			if  not self:isFriend(target)  then
				table.insert(targets,target)
			end
		end
	end
	local tt
	if #targets>0  then
		self:sort(targets, "handcard",true)
		tt= targets[1]
	elseif #badtargets>0 then
		tt= badtargets[1]
	elseif #ftargets>0 then
		tt=ftargets[1]
	else
		tt=target_table[1]
	end
	
	local _data = sgs.QVariant()
     _data:setValue(tt)
	self.player:setTag("pre_zhize",_data)
	return tt
end
sgs.ai_skill_askforag.aojiaofsuzhize = function(self, card_ids)
	local chosen_cards_id
	--Ϊʲô��table?
	local key_hearts={}
	local hearts={}
	for _,card_id in pairs(card_ids) do
		local card=sgs.Sanguosha:getCard(card_id)
		if card:getSuit()==sgs.Card_Heart and (card:isKindOf("Peach") or card:isKindOf("ExNihilo")) then
			table.insert(key_hearts,card_id)
		end
		if card:getSuit()==sgs.Card_Heart then
			table.insert(hearts,card_id)
		end
	end
	if #key_hearts>0 then
		chosen_card_id = key_hearts[1]
	elseif #hearts>0 then
		chosen_card_id = hearts[1]
	else
		chosen_card_id = card_ids[1]
	end
	if #hearts>1 then
		self.player:setTag("pre_heart",sgs.QVariant(true))
	end
	return chosen_card_id 
end
sgs.ai_playerchosen_intention.aojiaofsuzhize =function(self, from, to)
	local intention = 30
	if self:isFriend(from,to)   then
		local t=true
		for _,p in pairs (self:getEnemies(from)) do
			if not p:isKongcheng() then
				t=false
				break
			end
		end
		if t then intention = 0 end
	end
	sgs.updateIntention(from, to, intention)
end
--����Ϣ��ai
sgs.ai_skill_playerchosen.aojiaofsuchunxi = function(self, targets)
	local preheart=false
	local pre_zhize
	if self.player:getPhase()==sgs.Player_Draw then
		if self.player:getTag("pre_heart") and self.player:getTag("pre_heart"):toBool() then
			preheart=true
		end
		if preheart then
			pre_zhize=self.player:getTag("pre_zhize"):toPlayer()
		end
	end
	if pre_zhize then 
		self.player:setTag("pre_heart",sgs.QVariant(false))
		return pre_zhize 
	end
	target_table = self:getEnemies(self.player)
	if #target_table==0 then return false end
	local chunxi_target
	self:sort(target_table, "value")
	for _,target in pairs(target_table) do	
		if not chunxi_target and (not target:isKongcheng()) then
			chunxi_target=target
		else
			if (not target:isKongcheng()) and (self:isWeak(target) or target:getHandcardNum()<2) then
				chunxi_target=target
				break
			end
		end
	end
	if chunxi_target  then
		return chunxi_target
	end
end
sgs.ai_playerchosen_intention.aojiaofsuchunxi = 30
sgs.aojiaofsuchunxi_suit_value = {
	heart = 4.0
}
--��������ai
--������ʵ��ʹ�ò��Էǳ����ӡ�����
--����Ҫ�����ǲ���Ҫ���ִ�У�Ȼ�󻹵÷��������һ��Ǳ��
sgs.ai_skill_invoke.bllmwuyu = function(self) 
	--�غϿ�ʼ�׶εĻ�ñ��
	if self.player:getPhase() == sgs.Player_Start then return true end
end
function bllmwuyu_discard(player)
	local cards = sgs.QList2Table(player:getCards("he"))
    local all_hearts={}
	--self:sortByKeepValue(cards) -- ������ֵ����??
	for _,id in pairs(cards) do
		local card=sgs.Sanguosha:getCard(id)
		if card:getSuit()==sgs.Card_Heart and not (card:isKindOf("Peach") or card:isKindOf("DefensiveHorse"))then
			table.insert(all_hearts,id)
		end
	end
	return all_hearts
end
sgs.ai_skill_cardask["@bllm-discard"] = function(self, data)
    local prompt=self.player:getTag("wuyu_prompt"):toString() 
	if prompt=="bllmcaiyu" then
		if self.player:getMark("@yu")>=3 then return "." end
		local all_hearts=bllmwuyu_discard(self.player)
		if #all_hearts==0 then return "." end
        return "$" .. all_hearts[1]
	end
	if prompt=="bllmmingyu" then
		if self.player:getMark("@yu")>=2 then return "." end
		local all_hearts=bllmwuyu_discard(self.player)	
		if #all_hearts==0 then return "." end
		return "$" .. all_hearts[1]
	end
	if prompt=="bllmshuiyu" then
		if self.player:getHandcardNum()>self.player:getMaxCards() then
			num=self.player:getHandcardNum()-self.player:getMaxCards()
			if num<2 or self.player:getMark("@yu")>=1 then return "." end
			local all_hearts=bllmwuyu_discard(self.player)	
			if #all_hearts==0 then return "." end
			return "$" .. all_hearts[1]	
		end
	end
	return "."
end

sgs.ai_skill_invoke.bllm_useyu = function(self,data)
	local prompt=self.player:getTag("wuyu_prompt"):toString() 
	if prompt=="bllmcaiyu" then 
		return true
	end
	if prompt=="bllmmingyu" then
		local cards=self.player:getCards("j") 
		for _,card in sgs.qlist(cards) do
			if card:isKindOf("Indulgence") or card:isKindOf("SupplyShortage") then
				--��Ҫ�ж���аô?
				return true
			end
		end
	end
	if prompt=="bllmshuiyu" then
		if self.player:getHandcardNum()>self.player:getMaxCards() then
			if  self.player:getMark("@yu")>1 or self:isWeak(self.player) then
				return true
			end
		end
	end
	if prompt=="bllmshiyu" then
		local who=self.room:getCurrentDyingPlayer()
		if who and who:objectName()==self.player:objectName() then
			return true
		else
			return sgs.Slash_IsAvailable(self.player)
		end
	end
	if prompt=="bllmseyu" then
		return sgs.Slash_IsAvailable(self.player)
	end
end
--ɫ����ʳ����ʹ��ʱ���� 
sgs.ai_skill_choice.bllmwuyu= function(self)
	local cards=self.player:getCards("h")
	local t=sgs.Slash_IsAvailable(self.player)
	local slashs={}
	for _,c in sgs.qlist(cards) do
		if c:isKindOf("Slash") then
			table.insert(slashs,c)
		end
	end
	if t and #slashs>0 then
		return "bllmseyu"
	end
	if t==false and #slashs>1 then
		return "bllmseyu"
	end
	
	return "bllmshiyu"
end
--getturnuse���Ĳ���д ��ʱ����viewas�պ���  ����϶������ܲ���������
sgs.ai_view_as.bllmwuyu = function(card, player, card_place)
	local cards = player:getCards("h")
	local ecards={}
	for _,c in sgs.qlist(cards) do
		if c:isKindOf("BasicCard") then
			table.insert(ecards,c)
		end
	end
	if #ecards<2 then
		return false
	end
	local first_id = ecards[1]:getId()
	local second_id = ecards[2]:getId()
	return ("analeptic:bllmwuyu[%s:%s]=%d+%d"):format("to_be_decided", 0, first_id, second_id)
end
sgs.bllmwuyu_suit_value = {
	heart = 4.9
}

--��ǿ����ai
sgs.ai_skill_invoke.qiangyu = true
sgs.ai_skill_cardask["qiangyu_spadecard"] = function(self, data)
	--�������������ӱ���
	if self.player:getMark("@tianyi_Weapon")>0 then
		if self.player:getEquip(0):getSuit()==sgs.Card_Spade then
			return "$" ..self.player:getEquip(0):getId()
		end
	end
	if self.player:getMark("@tianyi_Armor")>0 then
		if self.player:getEquip(1):getSuit()==sgs.Card_Spade then
			return "$" ..self.player:getEquip(1):getId()
		end
	end
	if self.player:getMark("@tianyi_DefensiveHorse")>0 then
		if self.player:getEquip(2):getSuit()==sgs.Card_Spade then
			return "$" ..self.player:getEquip(2):getId()
		end
	end
	if self.player:getMark("@tianyi_OffensiveHorse")>0 then
		if self.player:getEquip(3):getSuit()==sgs.Card_Spade then
			return "$" ..self.player:getEquip(3):getId()
		end
	end
	
	--����������
	--��ʱû����̫�ࡣ����ǿ�����Ʋ���Ҫ��ôд�أ�
	for _, card in sgs.qlist(self.player:getHandcards()) do
		if card:getSuit()==sgs.Card_Spade  and (card:isKindOf("BasicCard"))  then
			return "$" ..card:getId()
		end
	end
	return "."
end
sgs.ai_skill_discard.qiangyu = sgs.ai_skill_discard.gamerule
sgs.qiangyu_suit_value = {
	spade = 4.9
}
--��ħ����ai
sgs.ai_skill_askforag.mokai = function(self, card_ids)
	local chosen_cards_id
	--��������  ʲô���͵�װ���������أ�
	for _,card_id in pairs(card_ids) do
		local card=sgs.Sanguosha:getCard(card_id)
		if card:getSuit()==sgs.Spade  then
			chosen_cards_id  =card_id
			break
		end
		
		if not chosen_card_id then
			chosen_cards_id  =card_id
		else	
			local card1=sgs.Sanguosha:getCard(chosen_cards_id)
			--װ����ʱ�ȱ���?
			if card1:isKindOf("Armor") and (not card:isKindOf("Armor")) then
				chosen_cards_id  =card_id		
			end
		end
	end

	return chosen_card_id 
end
--��ֹ���˳����õ�ɱ ��������׬�Ʋ
--����true��Ϊ��ֹ
sgs.ai_slash_prohibit.mokai = function(self, from, to, card)
	--û���ǽ䵶��askslashto�����
	local suit=card:getSuit()
	if to:getMark("@tianyi_Weapon")>0 and to:getEquip(0):getSuit()==suit then
		return self:isEnemy(to)
	end
	if to:getMark("@tianyi_Armor")>0 and to:getEquip(1):getSuit()==suit then
		return self:isEnemy(to)
	end
	if to:getMark("@tianyi_DefensiveHorse")>0 and to:getEquip(2):getSuit()==suit then
		return self:isEnemy(to)
	end
	if to:getMark("@tianyi_OffensiveHorse")>0 and to:getEquip(3):getSuit()==suit then
		return self:isEnemy(to)
	end
	return false
end


--����ֵ�趨
sgs.ai_chaofeng.zhu001 = 2
sgs.ai_chaofeng.zhu002 = 0
sgs.ai_chaofeng.zhu003 = 0
sgs.ai_chaofeng.zhu004 = 2
sgs.ai_chaofeng.zhu005 = 0
sgs.ai_chaofeng.zhu006 = 2
sgs.ai_chaofeng.zhu007 = -1
sgs.ai_chaofeng.zhu008 = 2
