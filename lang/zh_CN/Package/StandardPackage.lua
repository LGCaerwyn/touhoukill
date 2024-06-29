-- translation for StandardPackage

local t = {
	["standard_cards"] = "标准版",

	["slash"] = "杀",
	[":slash"] = "基本牌<br />使用时机：出牌阶段限一次。<br />使用目标：一名在你攻击范围内的其他角色。<br />作用效果：你对目标角色造成<font color=\"green\"><b>1点</b></font>伤害。",
	["slash-jink"] = "%src 对你使用了【杀】，请使用一张【闪】",
	["@multi-jink-start"] = "%src 对你使用了【杀】，你需依次使用 %arg 张【闪】",
	["@multi-jink"] = "%src 对你使用了【杀】，你需再依次使用 %arg 张【闪】",
	["@slash_extra_targets"] = "请选择此【杀】的额外目标",

	["jink"] = "闪",
	[":jink"] = "基本牌<br />使用时机：以你为目标的【杀】生效前。<br />使用目标：以你为目标的【杀】。<br />作用效果：抵消此【杀】。",
	["#NoJink"] = "%from 不能使用【<font color=\"yellow\"><b>闪</b></font>】响应此【<font color=\"yellow\"><b>杀</b></font>】",

	["peach"] = "桃",
	[":peach"] = "基本牌<br />使用方法①<br />使用时机：出牌阶段。<br />使用目标：一名包括你在内已受伤的角色（默认为你）。<br />作用效果：目标角色回复<font color=\"green\"><b>1点</b></font>体力。<br /> <br />使用方法②<br />使用时机：当一名角色处于濒死状态时。<br />使用目标：一名处于濒死状态的角色。<br />作用效果：目标角色回复<font color=\"green\"><b>1点</b></font>体力。",

	["Crossbow"] = "诸葛连弩",
	[":Crossbow"] = "装备牌·武器<br />攻击范围：1<br />装备技能：你使用【杀】无次数限制。",

	["DoubleSword"] = "雌雄双股剑",
	[":DoubleSword"] = "装备牌·武器<br />攻击范围：2<br />装备技能：当你使用【杀】指定一名目标角色后，若你与其所属势力不相同且其中一方所属的势力与主公相同时，你可以令其选择一项：弃置一张手牌；或令你摸一张牌。",
	["double-sword-card"] = "%src 发动了【雌雄双股剑】的效果，你须弃置一张手牌，或令 %src 摸一张牌",

	["QinggangSword"] = "青釭剑",
	[":QinggangSword"] = "装备牌·武器<br />攻击范围：2<br />装备技能：<font color=\"blue\"><b>锁定技，</b></font>当你使用【杀】指定一名目标角色后，其防具无效至：<br /><br />⑴ 此【杀】因对其无效而终止使用结算；<br />⑵ 此【杀】被其使用的【闪】抵消；<br />⑶ 此【杀】对其进行使用结算造成的伤害，在伤害结算中防止此伤害；<br />⑷ 此【杀】对其进行使用结算造成的伤害，在伤害结算中计算出最终的伤害值。",

	["Blade"] = "青龙偃月刀",
	[":Blade"] = "装备牌·武器<br />攻击范围：3<br />装备技能：当你使用【杀】指定目标后，你可以判定，若结果为红色，此【杀】的所有目标均不能使用【闪】响应此【杀】。",

	["Spear"] = "丈八蛇矛",
	[":Spear"] = "装备牌·武器<br />攻击范围：3<br />装备技能：你可以将两张手牌当【杀】使用或打出。",

	["Axe"] = "贯石斧",
	[":Axe"] = "装备牌·武器<br />攻击范围：3<br />装备技能：当你使用的【杀】被目标角色使用的【闪】抵消时，你可以弃置两张牌，令此【杀】依然对其生效。",
	["@Axe"] = "你可以弃置两张牌，令此【杀】依然对此目标角色生效",
	["~Axe"] = "选择两张牌 -> 确定",

	["Halberd"] = "方天画戟",
	[":Halberd"] = "装备牌·武器<br />攻击范围：4<br />装备技能：若你使用的【杀】是最后的手牌，此【杀】能额外选择至多两名目标。",

	["KylinBow"] = "麒麟弓",
	[":KylinBow"] = "装备牌·武器<br />攻击范围：５<br />装备技能：当你使用【杀】对目标角色造成伤害时，你可以弃置其装备区里的一张坐骑牌。",
	["KylinBow:dhorse"] = "+1坐骑",
	["KylinBow:ohorse"] = "-1坐骑",

	["EightDiagram"] = "八卦阵",
	[":EightDiagram"] = "装备牌·防具<br />装备技能：当你需要使用/打出【闪】时，你可以判定，若结果为红色，你视为使用/打出【闪】。",

	["standard_ex_cards"] = "标准版EX",

	["RenwangShield"] = "仁王盾",
	[":RenwangShield"] = "装备牌·防具<br />装备技能：<font color=\"blue\"><b>锁定技，</b></font>黑色【杀】对你无效。",

	["IceSword"] = "寒冰剑",
	[":IceSword"] = "装备牌·武器<br />攻击范围：２<br />装备技能：当你使用【杀】对目标角色造成伤害时，若其有牌，你可以防止此伤害，依次弃置其两张牌。",

	["Horse"] = "坐骑",
	[":+1 horse"] = "装备牌·坐骑<br />装备技能：其他角色与你的距离+1。",
	["JueYing"] = "绝影",
	["DiLu"] = "的卢",
	["ZhuaHuangFeiDian"] = "爪黄飞电",
	[":-1 horse"] = "装备牌·坐骑<br />装备技能：你与其他角色的距离-1。",
	["ChiTu"] = "赤兔",
	["DaYuan"] = "大宛",
	["ZiXing"] = "紫骍",

	["amazing_grace"] = "五谷丰登",
	[":amazing_grace"] = "锦囊牌·全体性的普通锦囊牌<br />使用时机：出牌阶段。<br />使用目标：所有角色。<br />执行动作：当此牌指定目标后，你亮出牌堆顶的X张牌（X为角色数）。<br />作用效果：目标获得这些牌中（剩余）的<font color=\"green\"><b>一张</b></font>牌。<br /><br />◆若你未将执行动作完整执行完毕，终止此牌的使用结算。<br />◆使用结算结束后，你将剩余的牌置入弃牌堆。<br />◆当此牌对目标角色生效后，若这些牌中无剩余的牌，则其不执行任何操作。",

	["god_salvation"] = "桃园结义",
	[":god_salvation"] = "锦囊牌·全体性的普通锦囊牌<br />使用时机：出牌阶段。<br />使用目标：所有角色。<br />作用效果：目标角色回复<font color=\"green\"><b>1点</b></font>体力。<br /><br />◆对未受伤的角色无效。",

	["savage_assault"] = "南蛮入侵",
	[":savage_assault"] = "锦囊牌·全体性的普通锦囊牌<br />使用时机：出牌阶段。<br />使用目标：所有其他角色。<br />作用效果：目标角色需打出<font color=\"green\"><b>一张</b></font>【杀】，否则受到你造成的<font color=\"green\"><b>1点</b></font>伤害。<br /><br />◆若改变此牌的第一个效果值，则目标执行此牌效果时，需依次打出X张【杀】（X为最终的效果值）。",
	["savage-assault-slash"] = "%src 使用了【南蛮入侵】，请打出 %dest 张【杀】来响应。 <br />本次询问 <font color=\"green\"><b>第%arg 张</b></font>【杀】",

	["archery_attack"] = "万箭齐发",
	[":archery_attack"] = "锦囊牌·全体性的普通锦囊牌<br />使用时机：出牌阶段。<br />使用目标：所有其他角色。<br />作用效果：目标角色需打出<font color=\"green\"><b>一张</b></font>【闪】，否则受到你造成的<font color=\"green\"><b>1点</b></font>伤害。<br /><br />◆若改变此牌的第一个效果值，则目标执行此牌效果时，需依次打出X张【闪】（X为最终的效果值）。",
	["archery-attack-jink"] = "%src 使用了【万箭齐发】，请打出 %dest 张【闪】以响应。 <br />本次询问 <font color=\"green\"><b>第%arg 张</b></font>【闪】",

	["collateral"] = "借刀杀人",
	[":collateral"] = "锦囊牌·非全体性的普通锦囊牌<br />使用时机：出牌阶段。<br />使用目标：一名装备区里有武器牌的其他角色。<br />作用效果：你选择目标角色攻击范围内的<font color=\"green\"><b>一名</b></font>角色，然后目标需对该角色使用【杀】，否则其将其装备区里的武器牌交给你。<br /><br />◆若此牌的效果值增加，则当此牌对目标角色生效后，你须选择其攻击范围内的X名角色（X为最终的效果值且至多等于其攻击范围），然后目标需对这些角色依次使用【杀】。<br />◆若目标角色未对你所选择的角色均使用【杀】，则其依然须将其装备区里的武器牌交给你。",
	["collateral-chooseVictim"] = "选择 <font color=\"green\"><b>%src</b></font> 执行【借刀杀人】时 使用【杀】的目标",
	["collateral-slash"] = "%dest 使用了【借刀杀人】，请对 %src 使用一张【杀】",
	["#CollateralSlash"] = "【借刀杀人】的目标角色 %from 需使用【<font color=\"yellow\"><b>杀</b></font>】的目标被指定为 %to",

	["duel"] = "决斗",
	[":duel"] = "锦囊牌·非全体性的普通锦囊牌<br />使用时机：出牌阶段。<br />使用目标：一名其他角色<br />作用效果：由目标角色开始，其与你轮流依次打出<font color=\"green\"><b>一张</b></font>【杀】，直到其中的一名角色未打出【杀】。然后未打出【杀】的角色受到另一名角色造成的<font color=\"green\"><b>1点</b></font>伤害。<br /><br />◆若改变此牌的第一个效果值，则目标执行此牌效果时，目标与你轮流依次打出X张【杀】（X为最终的效果值）。",
	["duel-slash"] = "%src 对你【决斗】，你需要打出 %dest 张【杀】。<br />本次询问 <font color=\"green\"><b>第%arg 张</b></font>【杀】",

	["ex_nihilo"] = "无中生有",
	[":ex_nihilo"] = "锦囊牌·非全体性的普通锦囊牌<br />使用时机：出牌阶段。<br />使用目标：一名包括你在内的角色。<br />作用效果：目标角色摸<font color=\"green\"><b>两张</b></font>牌。",

	["snatch"] = "顺手牵羊",
	[":snatch"] = "锦囊牌·非全体性的普通锦囊牌<br />使用时机：出牌阶段。<br />使用目标：一名区域里有牌且距离为1的其他角色。<br />作用效果：你获得目标角色的区域里的<font color=\"green\"><b>一张</b></font>牌",

	["dismantlement"] = "过河拆桥",
	[":dismantlement"] = "锦囊牌·非全体性的普通锦囊牌<br />使用时机：出牌阶段。<br />使用目标：一名区域里有牌的其他角色。<br />作用效果：你弃置其区域里的<font color=\"green\"><b>一张</b></font>牌",

	["nullification"] = "无懈可击",
	[":nullification"] = "锦囊牌·非全体性的普通锦囊牌<br />使用时机：一张锦囊牌对一个目标生效前。<br />使用目标：一张对一个目标生效前的锦囊牌。<br />作用效果：抵消此锦囊牌。",

	["indulgence"] = "乐不思蜀",
	[":indulgence"] = "锦囊牌·一次性的延时类锦囊牌<br />使用时机：出牌阶段。<br />使用目标：一名其他角色。<br />作用效果：目标角色判定，若结果不为<font size=\"5\", color=\"#FF0000\"><b>♥</b></font>，其跳过出牌阶段。",

	["lightning"] = "闪电",
	[":lightning"] = "锦囊牌·流动性的延时类锦囊牌<br />使用时机：出牌阶段。<br />使用目标：你。<br />作用效果：目标角色判定，若结果为<font size=\"5\", color=\"#808080\"><b>♠</b></font>2~9，其受到3点无来源的雷电伤害，将此【闪电】置入弃牌堆。<br /><br />◆【闪电】在使用结算结束后／目标角色被取消后，若对应的实体牌在处理区／判定区里，须将对应的实体牌置入其下家的判定区。若其下家不是此【闪电】的合法目标，则将对应的实体牌置入其下家的下家的判定区，以此类推。若所有角色均不是此【闪电】的合法目标，则将对应的实体牌置入其判定区。",

	["limitation_broken"] = "界限突破卡牌",

	["wooden_ox"] = "木牛流马",
	[":wooden_ox"] = "装备牌·宝物<br /><b>装备技能</b>：<br />" ..
					"1. <font color=\"green\"><b>出牌阶段限一次，</b></font>你可以将一张手牌扣置于【木牛流马】下称为“辎”，若如此做，你可以将装备区里的此装备牌置入一名其他角色的装备区。<br />" ..
					"2. 你能将“辎”如手牌般（转化）使用或打出。<br /><br />" ..
					"◆“辎”是移出游戏的牌，对你可见。<br />◆你装备区里的【木牛流马】进行移动结算时，若移往装备区，“辎”同时随之移转；若移往装备区以外的区域，则将“辎”置入弃牌堆。",
	["@wooden_ox-move"] = "你可以将【木牛流马】移动至一名其他角色的装备区",
	["#WoodenOx"] = "%from 使用/打出了 %arg 张 %arg2 牌",

	["BreastPlate"] = "护心镜",
	[":BreastPlate"] = "装备牌·防具<br /><b>装备技能</b>：当你受到伤害时，若此伤害值不小于你的体力值，你可以横置装备区里的此牌，防止此伤害。",
	["#BreastPlate"] = "%from 防止了 %to 对其造成的 %arg 点伤害[%arg2]",

	["Triblade"] = "三尖两刃刀",
	[":Triblade"] = "装备牌·武器<br />攻击范围：3<br/>装备技能：当你使用【杀】对目标角色造成伤害后，你可以弃置一张手牌并选择一名目标角色距离为1的其他角色，对其造成1点伤害。",
	["Tribladeskill"] = "三尖两刃刀",
	["@Triblade"] = "你可以发动【三尖两刃刀】技能",
	["~Triblade"] = "选择一张牌 -> 选择一名角色 -> 确定",

	["lure_tiger"] = "调虎离山",
	[":lure_tiger"] = "锦囊牌·非全体性的普通锦囊牌<br />使用时机：出牌阶段。<br />使用目标：一至两名其他角色。<br />作用效果：目标角色神隐，若如此做，其于此回合结束时解除神隐状态。<br />◆神隐的角色处于神隐状态（其不计入距离和座次的计算，不能使用牌，不是牌的合法目标，不进行伤害结算和体力流失结算）。<br /><br />重铸时机：出牌阶段。<br />◆重铸：声明一张牌并将之置入处理区，然后将之置入弃牌堆，最后摸一张牌。",

	["lure_tiger_effect"] = "调虎离山",
	["#lure_tiger-prohibit"] = "调虎离山",

	["drowning"] = "水淹七军",
	[":drowning"] = "锦囊牌·全体性的普通锦囊牌<br />使用时机：出牌阶段。<br />使用目标：所有其他角色。<br />作用效果：目标角色弃置其装备区里的<font color=\"green\"><b>一张</b></font>牌。<br /><br />◆对装备区里没有牌的角色无效。",
	["@drowning"] = "因%src的效果， 你须弃置装备区里的 %dest 张牌， 本次为第 %arg 次询问",

	["known_both"] = "知己知彼",
	[":known_both"] = "锦囊牌·非全体性的普通锦囊牌<br />使用时机：出牌阶段。<br />使用目标：一至两名有暗置手牌的其他角色。<br />作用效果：你明置目标角色<font color=\"green\"><b>一张</b></font>暗置手牌。<br />执行动作：此牌结算结束时，若你为当前回合角色，则其他角色于此回合内不能使用或打出其明置手牌。<br /><br />重铸时机：出牌阶段。<br />◆重铸：声明一张牌并将之置入处理区，然后将之置入弃牌堆，最后摸一张牌。",
	["#KnownBothLimit"] = "%from 以外的角色将于此回合内无法使用各自的明置手牌 " ,

	["saving_energy"] = "养精蓄锐",
	[":saving_energy"] = "锦囊牌·一次性的延时类锦囊牌<br />使用时机：出牌阶段。<br />使用目标：一名角色。<br />作用效果：目标角色判定，若结果不为<font size=\"5\", color=\"#808080\"><b>♠</b></font>，其跳过弃牌阶段，然后摸一张牌。",
	["saving_energy_effect"] = "养精蓄锐",

	["DeathSickle"] = "死神镰刀",
	[":DeathSickle"] = "装备牌·武器<br />攻击范围：2<br/>装备技能：当你使用【杀】指定一个目标后，若其体力下限为1，你可以令其体力下限于此【杀】结算完毕之前加1点。<br /><br />◆“体力下限”相当于角色扣减体力后进入濒死的最低阈值。角色扣减体力后若体力值低过这个阈值就会进入濒死了。<br />◆所有角色的体力下限默认为1。",
	["#DeathSickle"] = "%from 体力下限增加了%arg2 点， 当前体力下限 为 %arg 点。",
}

local ohorses = { "ChiTu", "DaYuan", "ZiXing"}
local dhorses = { "ZhuaHuangFeiDian", "DiLu", "JueYing", "HuaLiu" }

for _, horse in ipairs(ohorses) do
	t[":" .. horse] = t[":-1 horse"]
end

for _, horse in ipairs(dhorses) do
	t[":" .. horse] = t[":+1 horse"]
end

return t