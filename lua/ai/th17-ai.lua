
-- 【造形】出牌阶段开始时，你可以明置至少一张花色相同的手牌；当其他角色成为牌的唯一目标时，你可以重铸至少一张与之颜色相同的明置手牌，然后重铸牌数大于1，其摸一张牌。
-- 思路：亮牌的时候选牌最多的花色全亮算了，重铸的时候，队友重铸两张，非队友重铸一张，桃子不重铸

sgs.ai_skill_cardask["@zaoxing-show"] = function(self)
	local suits = {0,0,0,0}
	for _, c in sgs.qlist(self.player:getHandcards()) do
		if (c:getSuit() <= 3) and (not self.player:isShownHandcard(c:getId())) then
			suits[c:getSuit() + 1] = suits[c:getSuit() + 1] + 1
		end
	end
	
	local maxs = 1
	for i,n in ipairs(suits) do
		if suits[maxs] < n then maxs = i end
	end
	
	if suits[maxs] == 0 then return "." end
	
	local maxc = {}
	for _, c in sgs.qlist(self.player:getHandcards()) do
		if (c:getSuit() == maxs - 1) and (not self.player:isShownHandcard(c:getId())) then
			table.insert(maxc, tostring(c:getEffectiveId()))
		end
	end
	
	return "$" .. table.concat(maxc, "+")
end

sgs.ai_skill_cardask["@zaoxing-recast"] = function(self, data)
	local zxproperty = self.player:property("zaoxing2"):toString()
	local zxsuit = ((zxproperty == "black") and sgs.Card_Black or ((zxproperty == "red") and sgs.Card_Red or sgs.Card_Colorless))
	local cards = {}
	for _, id in sgs.qlist(self.player:getShownHandcards()) do
		local c = sgs.Sanguosha:getCard(id)
		if (c:getColor() == zxsuit) and (not c:isKindOf("Peach")) then table.insert(cards, c) end
	end
	
	if #cards == 0 then return "." end
	
	self:sortByKeepValue(cards)
	local r = "$" .. tostring(cards[1]:getId())
	
	local use = data:toCardUse()
	if self:isFriend(use.to:first()) and (#cards >= 2) then
		r = r .. "+" .. tostring(cards[2]:getId())
	end
	
	return r
end

-- 【灵守】结束阶段开始时，你可以观看一名角色的手牌并展示其中至多两张花色相同的牌，其须选择将这些牌重铸或当【杀】使用。
-- 思路：找个牌最多的拆个桃子啥的

sgs.ai_skill_playerchosen.lingshou = function(self, targets)
	local n = {}
	for _, p in sgs.qlist(targets) do
		if self:isEnemy(p) then table.insert(n, p) end
	end
	
	if #n == 0 then return nil end
	
	local mostp =1
	for _, p in ipairs(n) do
		if p:getHandcardNum() > n[mostp]:getHandcardNum() then mostp = _ end
	end
	
	return n[mostp]
end

sgs.ai_skill_askforag.lingshou = function(self, ids)
	local cards = {}
	for _, id in ipairs(ids) do
		table.insert(cards, sgs.Sanguosha:getCard(id))
	end
	
	self:sortByKeepValue(cards)
	return cards[#cards]:getId()
end

-- 用默认的使用杀算法选择目标杀
sgs.ai_skill_use["@@LingshouOtherVS"] = function(self)
	local idstrings = string.split(self.player:property("lingshouSelected"):toString(), "+")
	local slash = sgs.Sanguosha:cloneCard("Slash", sgs.Card_SuitToBeDecided, -1)
	slash:setSkillName("_lingshou")
	for _, s in ipairs(idstrings) do
		slash:addSubcard(tonumber(s))
	end
	
	local use = {isDummy = true, to = sgs.SPlayerList()}
	self:useCardSlash(slash, use)
	slash:deleteLater()
	if use.card and not use.to:isEmpty() then
		local slist = {}
		for _, s in sgs.qlist(use.to) do
			table.insert(slist, s:objectName())
		end
		return slash:toString() .. "->" .. table.concat(slist, "+")
	end
	
	return "."
end

-- 【祷应】主公技，其他鬼势力角色的出牌阶段限一次，其可以选择一名角色为目标并将一张手牌交给你，你可以令所有其他角色于此回合内与目标距离-1。
-- 目前不好用，搁置
-- zhuying目前没人发动？啥鬼？
local zhuying_s={}
zhuying_s.name="zhuying_attach"
table.insert(sgs.ai_skills,zhuying_s)
zhuying_s.getTurnUseCard=function(self)
	if self.player:getKingdom() ~= "gxs" then return nil end
	if self.player:isKongcheng() then return nil end
	local flag = false
	for _, p in sgs.qlist(self.player:getAliveSiblings()) do
		if p:hasLordSkill("zhuying") and (not p:hasFlag("zhuying_selected")) and self:isFriend(p) then
			flag = true
			break
		end
	end
	if not flag then return nil end

	local cards = self.player:getHandcards()
	local cl = {}
	for _, c in sgs.qlist(cards) do
		if not (c:isKindOf("Peach")) then
			if (c:isKindOf("Snatch") or c:isKindOf("Slash") or c:isKindOf("SupplyShortage")) then
				if #self:getCards(c:objectName()) >= 2 then
					table.insert(cl, c)
				end
			else
				table.insert(cl, c)
			end
		end
	end
	if #cl == 0 then return nil end
	
	self:sortByUseValue(cl)
	return sgs.Card_Parse("@ZhuyingCard:" .. tostring(cl[1]:getId()))
end

sgs.ai_skill_use_func.ZhuyingCard = function(card, use, self)
	local ps = {}
	if (#self:getCards("Snatch") > 0 or #self:getCards("SupplyShortage") > 0) then
		for _, p in sgs.qlist(self.room:getOtherPlayers(self.player)) do
			if (self.player:distanceTo(p) == 2) and self:isEnemy(p) then table.insert(ps,p) end
		end
	end
	
	if #ps == 0 then
		if #self:getCards("Slash") > 0 then
			for _, p in sgs.qlist(self.room:getOtherPlayers(self.player)) do
				if (self.player:distanceTo(p) == self.player:getAttachRange() + 1) and self:isEnemy(p) then table.insert(ps,p) end
			end
		end
	end
	
	if #ps == 0 then return end
	self:sortEnemies(ps)
	use.card = card
	if use.to then use.to:append(ps[1]) end
end

sgs.ai_use_priority.ZhuyingCard =  sgs.ai_use_priority.Snatch + 0.001

sgs.ai_skill_playerchosen.zhuying_multilord = function(self,targets)
	for _, p in sgs.qlist(targets) do
		if self:isFriend(p) then return p end
	end
end

sgs.ai_skill_invoke.zhuying_effect = function(self, data)
	local p = self.room:findPlayerByObjectName(string.sub(data:toString(), 8))
	if p then return self:isFriend(p) end
	return false
end

-- 【善垒】锁定技，回合开始时，若你的手牌数大于你的手牌上限，你将手牌弃置至上限；回合结束时，若你的手牌数不大于所有其他角色，你摸至X张（X为手牌最多的角色的手牌数+1）。
-- None needed

-- 【崩落】一名角色的一个阶段结束时，若你于此阶段内有过不因使用而失去牌后使你的手牌数大于你的手牌上限的情况，你可以将一张手牌当【杀】使用。当此牌对目标造成伤害时，若其手牌数小于你，你可以将手牌调整至你的手牌上限，令此伤害值+1。
-- 用最坏的牌和默认的杀算法选择目标来杀就行，用弃牌阶段弃牌的算法选择弃牌调整所需的牌

sgs.ai_skill_use["@@bengluo-card1"] = function(self)
	local cards = self.player:getHandcards()
	local cl = {}
	for _, c in sgs.qlist(cards) do
		if not (c:isKindOf("Peach")) then
			table.insert(cl,c)
		end
	end
	if #cl == 0 then return nil end
	self:sortByKeepValue(cl)
	
	for _, c in ipairs(cl) do
		local u = {isDummy=true, to = sgs.SPlayerList()}
		local slash = sgs.Sanguosha:cloneCard("Slash", sgs.Card_SuitToBeDecided, -1)
		slash:addSubcard(c)
		slash:setSkillName("bengluo")
		slash:deleteLater()
		self:useCardSlash(slash, u)
		if u.card and (not u.to:isEmpty()) then
			local slist = {}
			for _, s in sgs.qlist(u.to) do
				table.insert(slist, s:objectName())
			end
			return slash:toString() .. "->" .. table.concat(slist, "+")
		end
	end
	
	return "."
end

local bengluoNeedAddDamage=function(self, player)
	return self:isEnemy(player)
end

sgs.ai_skill_cardask["@bengluo-discard"] = function(self, data)
	local damage = data:toDamage()
	if bengluoNeedAddDamage(self, damage.to) then
		local n = self.player:getHandcardNum() - self.player:getMaxCards()
		local cards = sgs.ai_skill_discard.gamerule(self, n, n)
		local cardss = {}
		for _, i in ipairs(cards) do
			table.insert(cardss, tostring(i))
		end
		
		return "$" .. table.concat(cardss, "+")
	end
end

sgs.ai_skill_invoke.bengluo = function(self, data)
	local damage = data:toDamage()
	return bengluoNeedAddDamage(self, damage.to)
end

-- 【沦溺】其他角色回合开始时，你可以将一张装备牌置入其装备区（若已有同类型的牌则替换之），若如此做，此回合的：出牌段结束时，其获得其装备区里所有的牌；弃牌阶段结束时，你可以获得一张其于此阶段内弃置于弃牌堆里的装备牌。
-- 看注释吧

-- 回合开始的询问
sgs.ai_skill_use["@@lunni"] = function(self)
	local w, a, oh, dh, t = {},{},{},{},{}
	for _, p in sgs.qlist(self.player:getCards("hes")) do
		    if p:isKindOf("Weapon") then table.insert(w, p:getId())
		elseif p:isKindOf("Armor") then table.insert(a, p:getId())
		elseif p:isKindOf("OffensiveHorse") then table.insert(oh, p:getId())
		elseif p:isKindOf("DefensiveHorse") then table.insert(dh, p:getId())
		elseif p:isKindOf("Treasure") then table.insert(t, p:getId())
		end
	end

	local uselessCamouflage = function(room, player)
		for _, p in sgs.qlist(room:getOtherPlayers()) do
			if p:getArmor() then return true end
		end
		return false
	end

	local id

	local __first = true
	while __first do -- do {} while (0);
		__first = false
		local current = self.room:getCurrent()
		if self:isFriend(current) then
			-- 队友的话
			-- 送狮子回血
			if (not current:getArmor()) or ((current:getArmor():getClassName() == "Camouflage") and uselessCamouflage(self.room,current)) then
				local silverlion
				for _, p in ipairs(a) do
					if sgs.Sanguosha:getCard(p):getClassName() == "SilverLion" then silverlion = p break end
				end
				if silverlion then
					if self:isWeak() and self.player:getArmor() and self.player:getArmor():getId() == silverlion and current:getCards("hes"):length() <= current:getMaxCards() then
						id = silverlion
						break
					end
					if self:isWeak(current) then
						id = silverlion
						break
					end
				end
			end
			-- 送连弩
			if getKnownCard(current, self.player, "Slash", true) > 2 then
				if not current:getWeapon() and #w > 0 then
					local crossbow
					for _, p in ipairs(w) do
						if sgs.Sanguosha:getCard(p):getClassName() == "Crossbow" then silverlion = p break end
					end
					if crossbow then
						id = crossbow
						break
					end
				end
			end
			-- 不要让队友弃牌太多
			if current:getCards("hes"):length() <= current:getMaxCards() then
				-- 送距离
				if getKnownCard(current, self.player, "Slash", true) > 0 then
					if not current:getWeapon() and #w > 0 then
						local maxDist = 1
						for _, id in ipairs(w) do
							local weapon = sgs.Sanguosha:getCard(id):getRealCard():toWeapon()
							if sgs.Sanguosha:getCard(w[maxDist]):getRealCard():toWeapon():getRange() < weapon:getRange() then maxDist = _ end
						end
						id = w[maxDist]
						break
					elseif not current:getOffensiveHorse() and #of > 0 then
						id = of[1]
						break
					end
				end
				-- 送玉玺
				if not current:getTreasure() and #t > 0 then
					local jadeseal
					for _, p in ipairs(t) do
						if sgs.Sanguosha:getCard(p):getClassName() == "JadeSeal" then jadeseal = p break end
					end
					if jadeseal then
						id = jadeseal
						break
					end
				end
			end
		elseif self:isEnemy(current) then
			-- 如果是对手，优先拆母牛
			if current:getTreasure() and (current:getTreasure():getClassName() == "WoodenOx") and #t > 0 then
				-- 给个宝塔镇一镇（
				local pagoda
				for _, p in ipairs(t) do
					if sgs.Sanguosha:getCard(p):getClassName() == "Pagoda" then pagoda = p break end
				end
				if pagoda then
					id = pagoda
					break
				end
			end
			-- 弄掉对面的防御装备，限制对面的母牛
			if (not ((not current:getArmor()) or ((current:getArmor():getClassName() == "Camouflage") and uselessCamouflage(self.room,current)))) or current:getDefensiveHorse() or (current:getTreasure() and (current:getTreasure():getClassName() == "WoodenOx")) then
				if #a > 0 then
					-- 可以给的防具，除了白银狮子都能给？
					local theArmor
					for _, p in ipairs(a) do
						-- 给手牌
						if sgs.Sanguosha:getCard(p):getClassName() ~= "SilverLion" and (not self.player:getArmor() or (self.player:getArmor():getId() ~= p)) then theArmor = p break end
					end
					if theArmor then
						id = theArmor
						break
					end
				end
				if #dh > 0 then
					-- +1马
					local theDh
					for _, p in ipairs(dh) do
						-- 给手牌
						if not self.player:getDefensiveHorse() or (self.player:getDefensiveHorse():getId() ~= p) then theDh = p break end
					end
					if theDh then
						id = theDh
						break
					end
				end
			end
			-- 给个连弩限制距离，但是前提是他得没有杀和母牛
			if current:getWeapon() --[[ or (current:getTreasure() and (current:getTreasure():getClassName() == "WoodenOx")) ]] then
				if getKnownCard(current, self.player, "Slash", true) == 0 and (not (current:getTreasure() and (current:getTreasure():getClassName() == "WoodenOx"))) then
					local crossbow
					for _, p in ipairs(w) do
						if sgs.Sanguosha:getCard(p):getClassName() == "Crossbow" then crossbow = p break end
					end
					if crossbow then
						id = crossbow
						break
					end
				end
			end
			
			if current:getCards("hes"):length() > current:getMaxCards() then
				-- 拆迁，送-1
				if #oh > 0 then
					local theOh
					for _, p in ipairs(oh) do
						-- 优先给手牌
						if not self.player:getOffensiveHorse() or (self.player:getOffensiveHorse():getId() ~= p) then theOh = p break end
					end
					if theOh then
						id = theOh
						break
					end
					-- 装备也给
					id = oh[1]
					break
				end
				-- 送宝塔
				if #t > 0 then
					local pagoda
					for _, p in ipairs(t) do
						if sgs.Sanguosha:getCard(p):getClassName() == "Pagoda" then pagoda = p break end
					end
					if pagoda then
						id = pagoda
						break
					end
				end
			end
		end
	end
	
	if id then
		local c = sgs.Sanguosha:cloneSkillCard("LunniCard")
		c:addSubcard(id)
		c:deleteLater()
		return c:toString() .. "->."
	end
	
	return "."
end

-- 弃牌阶段捡垃圾
sgs.ai_skill_askforag.lunni = function(self, ids)
	local cards = {}
	for _, id in ipairs(ids) do
		table.insert(cards, sgs.Sanguosha:getCard(id))
	end
	
	-- 狮子
	for _, c in ipairs(cards) do
		if c:getClassName() == "SliverLion" then return c:getId() end
	end
	
	-- 母牛
	for _, c in ipairs(cards) do
		if c:getClassName() == "WoodenOx" then return c:getId() end
	end
	
	-- 连弩
	for _, c in ipairs(cards) do
		if c:getClassName() == "Crossbow" then return c:getId() end
	end
	
	self:sortByUseValue(cards, true)
	
	-- 其他装备：优先度：宝物，防具，武器，+1，-1
	for _, c in ipairs(cards) do
		if c:isKindOf("Treasure") then return c:getId() end
	end
	for _, c in ipairs(cards) do
		if c:isKindOf("Armor") then return c:getId() end
	end
	for _, c in ipairs(cards) do
		if c:isKindOf("Weapon") then return c:getId() end
	end
	for _, c in ipairs(cards) do
		if c:isKindOf("DefensiveHorse") then return c:getId() end
	end
	for _, c in ipairs(cards) do
		if c:isKindOf("OffensiveHorse") then return c:getId() end
	end
	
	-- 理论上不会走到这里的
	return ids[1]
end

-- 【劝归】当其他角色因牌的效果受到大于1点的伤害而进入濒死状态时，你可以展示并获得其区域里的一张牌，若获得的是装备牌，其将体力回复至其体力下限。
-- 思路：拿队友装备（优先级：连弩，距离2武器，-1，其他武器，+1，宝物，防具）和兵乐电，拿对手手牌和春息养精蓄锐

sgs.ai_skill_invoke.quangui = function(self, data)
	local dying = data:toDying()
	if self:isFriend(dying.who) then
		if not dying.who:getEquips():isEmpty() then
			if dying.who:getWeapon() and dying.who:getWeapon():getClassName() == "Crossbow" then sgs.ai_skill_cardchosen.quangui = dying.who:getWeapon():getId() return true end
			if dying.who:getWeapon() and dying.who:getWeapon():getRealCard():toWeapon():getRange() == 2 then sgs.ai_skill_cardchosen.quangui = dying.who:getWeapon():getId() return true end
			if dying.who:getOffensiveHorse() then sgs.ai_skill_cardchosen.quangui = dying.who:getOffensiveHorse():getId() return true end
			if dying.who:getWeapon() then sgs.ai_skill_cardchosen.quangui = dying.who:getWeapon():getId() return true end
			if dying.who:getDefensiveHorse() then sgs.ai_skill_cardchosen.quangui = dying.who:getDefensiveHorse():getId() return true end
			if dying.who:getTreasure() then sgs.ai_skill_cardchosen.quangui = dying.who:getTreasure():getId() return true end
			if dying.who:getArmor() then sgs.ai_skill_cardchosen.quangui = dying.who:getArmor():getId() return true end
		end
		if not dying.who:getJudgingArea():isEmpty() then
			local ids = {}
			for _, p in sgs.qlist(dying.who:getJudgingArea()) do
				ids[p:getClassName()] = p:getEffectiveId()
			end
			if ids.Indulgence then sgs.ai_skill_cardchosen.quangui = ids.Indulgence return true end
			if ids.SupplyShortage then sgs.ai_skill_cardchosen.quangui = ids.SupplyShortage return true end
			if ids.Lightning then sgs.ai_skill_cardchosen.quangui = ids.Lightning return true end
			-- if ids.SavingEnergy then sgs.ai_skill_cardchosen.quangui = ids.SavingEnergy return true end
			-- if ids.SpringBreath then sgs.ai_skill_cardchosen.quangui = ids.SpringBreath return true end
		end
	elseif self:isEnemy(dying.who) then
		if dying.who:getHandcardNum() > 0 then sgs.ai_skill_cardchosen.quangui = -1 return true end
		if not dying.who:getJudgingArea():isEmpty() then
			local ids = {}
			for _, p in sgs.qlist(dying.who:getJudgingArea()) do
				ids[p:getClassName()] = p:getEffectiveId()
			end
			-- if ids.Indulgence then sgs.ai_skill_cardchosen.quangui = ids.Indulgence return true end
			-- if ids.SupplyShortage then sgs.ai_skill_cardchosen.quangui = ids.SupplyShortage return true end
			-- if ids.Lightning then sgs.ai_skill_cardchosen.quangui = ids.Lightning return true end
			if ids.SpringBreath then sgs.ai_skill_cardchosen.quangui = ids.SpringBreath return true end
			if ids.SavingEnergy then sgs.ai_skill_cardchosen.quangui = ids.SavingEnergy return true end
		end
	end
	
	return false
end


--[[
kutaka 4血
【狱守】 其他角色的准备阶段开始时，你可以弃置一张手牌，若如此做，当其于此回合内使用第一张有点数的牌时，若点数不大于之，此牌无效，反之此牌不计入限制的使用次数。
【灵渡】 当你区域里的牌于回合外置入弃牌堆后（包括牌使用或打出结算完毕后），你可以用你区域里另一张牌替换之，若以此法失去了一个区域里最后的一张牌，你摸一张牌。每回合限一次。

yachie 4血
【夺志】锁定技 当你使用牌结算完毕后，你令所有其他角色于当前回合内不能使用或打出牌。

mayumi 4血
【领军】当你于一个回合内使用的第一张【杀】结算完毕后，你可以选择此牌的一个目标，令攻击范围内有其的其他角色各选择是否将一张基本牌当【杀】对其使用（除其外的角色不是合法目标），然后若此次被以此法转化的牌均是【杀】，你视为对其使用【杀】。
【瓷偶】锁定技 当你受到伤害时，若受到的是【杀】造成的无属性伤害，此伤害结算结束后你失去1点体力，否则此伤害值-1。

saki 4血
【劲疾】锁定技 你与其他角色的距离-X（X为你装备区里的牌数），其他角色与你的距离+Y（Y为你装备区里横置的牌数）。
【天行】一名其他角色的准备阶段开始时，你可以横置装备区里的一张牌，视为对其使用【杀】；当你使用【杀】对一名角色造成伤害后，其于此回合内不能使用以你为唯一目标的牌。
]]
