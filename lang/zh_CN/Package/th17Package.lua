return {
	["gxs"] = "鬼",
	["th17"] = "鬼形兽",

--***********************************
	
	["keiki"] = "埴安神袿姬" ,
	["#keiki"] = "孤立无援造就的造形神" ,
	["zaoxing"] = "造形",
	[":zaoxing"] = "回合开始时，你可声明一个与上回合不同的花色，直到你下个回合开始前，其他角色使用此花色的牌时，须选择一项：交给你一张同花色的牌，或令你令此牌无效并获得之。",
	["@zaoxing-discard"] = "%src 的造型你须选择交给 %src 一张 %arg2 牌，或你令此 %arg 无效并令 %src 获得之",
	["lingshou"] = "灵守",
	[":lingshou"] = "出牌阶段结束后，你可观看一名角色的手牌，并指定其中至少一张同花色的牌。然后你声明一张基本牌和其使用此牌的合法目标，令其选择是否弃置你指定的牌并视为使用之。",
	["zhuying"] = "祷应",
	[":zhuying"] = "主公技，每名鬼势力的角色出牌阶段限一次，其可选择一名角色并交给你一张手牌，令你可令本回合内其他角色与目标距离-1。",
	
	["eika"] = "戎璎花" ,
	["#eika"] = "河原的偶像水子" ,
	
	["shanlei"] = "善垒",
	[":shanlei"] = "回合开始时，若你手牌数大于手牌上限，你弃置手牌至上限；回合结束时，若你手牌数不是唯一最多，你摸牌至手牌唯一最多。", -- 无锁定技标签，但是必定发动，因为描述中没有“可以”
	["@shanlei-discard"] = "善垒你手牌数大于手牌上限，你弃置手牌至上限",
	["bengluo"] = "崩落",
	[":bengluo"] = "一名角色的一个阶段结束后，若你此阶段因使用以外的原因失去至少一张牌，且失去至少其中一次牌后你的手牌数大于手牌上限，你可以将一张手牌当【杀】使用。此【杀】对目标造成伤害时，若此【杀】的目标手牌数小于你，你可调整手牌至上限，令此【杀】伤害+1。",
	["@bengluo-kill"] = "崩落你可以将一张手牌当杀使用",
	["@bengluo-discard"] = "崩落你可调整手牌至上限令杀伤害+1，你可以弃置 %arg 张牌",
	["bengluo:@bengluo-draw"] = "崩落你可调整手牌至上限令杀伤害+1，你可以摸 %arg 张牌",
	
	["urumi"] = "牛崎润美" ,
	["#urumi"] = "古代鱼的携子护卫" ,
	
	["lunni"] = "沦溺",
	[":lunni"] = "其他角色回合开始时，你可将一张你手牌中或装备区内的装备牌置入其装备区（可替换已有装备）。若如此做，则其于出牌阶段结束后获得其装备区中的所有牌，且你可于其弃牌阶段结束后获得弃牌堆中其于此阶段弃置的一张装备牌。",
	["@lunni-discard"] = "沦溺你可以将一张装备置入 %src 的装备区（可替换已有装备）",
	["quangui"] = "劝归",
	[":quangui"] = "其他角色因牌受到伤害进入濒死状态时，若该伤害大于1，你可展示并获得其区域中一张牌。若此牌是装备牌，你令其体力回复至下限。"

	["kutaka"] = "庭渡久侘歌" ,
	["#kutaka"] = "地狱口岸的守护神" ,
	["yvshou"] = "狱守",
	[":yvshou"] = " 一名其他角色回合开始时，你可弃置一张手牌。若该角色于此回合使用的第一张有点数的牌的点数不大于之，则你令此牌无效；否则你令此牌不计入次数限制。",
	["lingdu"] = "灵渡",
	[":lingdu"] = "每回合限一次，你区域内的牌于回合外置入弃牌堆后或因使用或打出而置入弃牌堆后，你可选择自己区域内另一张牌替换之。若以此法失去了一个区域内最后一张牌，你摸一张牌。",
	
	["yachie"] = "吉吊八千慧" ,
	["#yachie"] = "鬼杰组组长" ,
	["duozhi"] = "夺志",
	[":duozhi"] = "锁定技 你使用或打出的牌结算后，此回合内其他角色不能使用或打出牌。",
	
	["mayumi"] = "杖刀偶磨弓" ,
	["#mayumi"] = "埴轮兵长" ,
	["lingjun"] = "领军",
	[":lingjun"] = "你每回合使用的第一张【杀】结算后，你可选择此【杀】的一个目标，令攻击范围内有其的其他角色可将一张基本牌当【杀】对其使用（该目标以外的角色不是合法目标），然后若以此法转化的所有牌（至少一张）于转化前为【杀】，你视为对目标使用一张【杀】。",
	["ciou"] = "瓷偶",
	[":ciou"] = "锁定技 你受到伤害时，若造成伤害的牌为无属性【杀】，你于伤害结算结束后失去1点体力，否则此伤害值-1。",
	
	["saki"] = "骊驹早鬼" ,
	["#saki"] = "劲牙组组长" ,
	["jinji"] = "劲疾",
	[":jinji"] = "锁定技 你与其他角色的距离 -x （x为你装备区的装备牌数量），其他角色与你的距离 +y（y为你装备区横置的牌的数量）。",
	["tianxing"] = "天行",
	[":tianxing"] = "一名其他角色回合开始时，你可横置一张装备牌，视为你对其使用了一张【杀】（无距离限制）。你的【杀】对一名角色造成伤害后，该角色此回合内无法使用以你为唯一目标的牌。",
}
