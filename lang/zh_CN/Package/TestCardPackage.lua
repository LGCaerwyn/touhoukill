-- translation for ManeuveringPackage

return {
	["test_card"] = "东方杀测试牌",

	["light_slash"] = "秽杀",
	[":light_slash"] = "基本牌<br />使用时机：出牌阶段限一次。<br />使用目标：你攻击范围内的一名其他角色。<br />作用效果：你明置目标角色的<font color=\"green\"><b>一张</b></font>手牌，然后对其造成<font color=\"green\"><b>1点</b></font>伤害。",

	["power_slash"] = "幻杀",
	[":power_slash"] = "基本牌<br />使用时机：出牌阶段限一次。<br />使用目标：你攻击范围内的一名其他角色。<br />作用效果：你横置目标角色装备区里的<font color=\"green\"><b>一张</b></font>牌，然后对其造成<font color=\"green\"><b>1点</b></font>伤害。",

	["iron_slash"] = "缚杀",
	[":iron_slash"] = "基本牌<br />使用时机：出牌阶段限一次。<br />使用目标：你攻击范围内的一名其他角色。<br />作用效果：你令目标角色横置，然后对其造成<font color=\"green\"><b>1点</b></font>伤害。",

	["magic_analeptic"] = "果酒",
	[":magic_analeptic"] = "基本牌<br />使用方法①<br />使用时机：出牌阶段，每回合限一次。<br />使用目标：包括你在内的一名角色。<br />作用效果：目标角色于此回合内使用的下一张普通锦囊牌的第一个效果值+<font color=\"green\"><b>1</b></font>。<br />◆不处于濒死状态的角色使用【酒】默认是以使用方法①使用【酒】。<br /> <br />使用方法②<br />使用时机：当你处于濒死状态时。<br />使用目标：你。<br />作用效果：你回复<font color=\"green\"><b>1点</b></font>体力。<br />◆处于濒死状态的角色使用【酒】默认是以使用方法②使用【酒】。",

	["super_peach"] = "仙桃",
	[":super_peach"] = "基本牌<br />使用方法①<br />使用时机：出牌阶段。<br />使用目标：一名处于异常状态的角色。<br />作用效果：目标角色暗置其所有明置手牌，然后重置并重置装备区里的所有横牌，最后回复<font color=\"green\"><b>1点</b></font>体力。<br />◆若一名角色的人物牌处于连环状态或其装备区里有横牌或其手牌里有明牌，则该角色处于异常状态。 <br /><br />使用方法②<br />使用时机：当一名角色处于濒死状态时。<br />使用目标：一名处于濒死状态的角色。<br />作用效果：目标角色暗置其所有明置手牌，然后重置并重置装备区里的所有横牌，最后回复<font color=\"green\"><b>1点</b></font>体力。",

	["chain_jink"] = "磁闪",
	[":chain_jink"] = "基本牌<br />使用时机：以你为目标的【杀】生效前。<br />使用目标：以你为目标的【杀】。<br />作用效果：你令此【杀】的使用者横置，然后抵消此【杀】。",

	["light_jink"] = "光闪",
	[":light_jink"] = "基本牌<br />使用时机：以你为目标的【杀】生效前。<br />使用目标：以你为目标的【杀】。<br />作用效果：你明置此【杀】的使用者的一张手牌，然后抵消此【杀】。",

	["Camera"] = "照相机",
	[":Camera"] = "装备牌·武器<br />攻击范围：4<br />装备技能：当你使用【杀】对目标角色造成伤害后，若其有手牌，你可以依次明置其两张手牌。",

	["Gun"] = "狂气之枪",
	[":Gun"] = "装备牌·武器<br />攻击范围：4<br />装备技能：<font color=\"blue\"><b>锁定技，</b></font>当你使用【杀】对其他角色造成伤害后，横置其装备区里的所有牌。",
	["#Gun"]= "%from 对 %to 触发了 %arg 的效果",
	
	["JadeSeal"] = "玉玺",
	[":JadeSeal"] = "装备牌·宝物<br />装备技能：出牌阶段限一次。你可以视为使用【知己知彼】。",
	["~JadeSeal"] = "选择【知己知彼】的合法目标 -> 确定",
	
	
    ["Camouflage"] = "光学迷彩",
	[":Camouflage"] = "装备牌·防具<br />装备技能：<font color=\"blue\"><b>锁定技，</b></font>当你受到伤害时，若场上仅有一张防具牌，防止此伤害；当此牌因使用结算完毕而置入你的装备区后，你弃置场上一张与之牌名不同的防具牌。",
    ["#Camouflage"] = "%from 的防具【%arg2】防止了 %arg 点伤害",
	["@camouflage"] = "光学迷彩： 请选择一名装备区里有防具牌的其他角色，弃置其装备区里的防具牌",
	
	["await_exhausted"] = "以逸待劳",
	[":await_exhausted"] = "锦囊牌·非全体性的普通锦囊牌<br />使用时机：出牌阶段。<br />使用目标：一至两名角色。<br />作用效果：目标角色摸<font color=\"green\"><b>两张</b></font>牌，然后弃置<font color=\"green\"><b>两张</b></font>牌。",
	
	["spell_duel"] = "符斗",
	[":spell_duel"] = "锦囊牌·非全体性的普通锦囊牌<br />使用时机：出牌阶段。<br />使用目标：一名有手牌的角色<br />作用效果：你与其拼点：当你赢后，你对其造成<font color=\"green\"><b>1</b></font>点伤害；当你没赢后，你令其对你造成<font color=\"green\"><b>1</b></font>点伤害。",
	
	["kusuri"] = "药",
	[":kusuri"] = "基本牌<br />使用方法①<br />使用时机：出牌阶段。<br />使用目标：包括你在内的一名处于异常状态（包含人物牌背面朝上、人物牌处于连环状态、装备区里有横置的牌、手牌里有明置的手牌）的角色。<br />作用效果：若目标角色：人物牌背面朝上，其翻面；处于连环状态，重置；装备区里有横置的牌，重置这些牌；手牌里有明置的手牌，暗置这些牌）"
	.."<br /><br />使用方法②：<br />使用时机：当一名角色处于濒死状态时。<br />使用目标：一名处于濒死状态的角色。<br />作用效果：目标角色回复1点体力。",
	
	["Pagoda"] = "宝塔",
	[":Pagoda"] = "装备牌·宝物<br />装备技能：你可以将一张黑色手牌当【无懈可击】使用。每阶段限一次；<br />当你使用【无懈可击】抵消目标牌后，你可以令选择目标牌所指定的所有目标角色，然后所有角色不能使用【无懈可击】响应对这些角色结算的目标牌，若如此做，当此牌对你选择的这些角色中的一名角色生效前，抵消之。",
    ["#PagodaNullified"] = " 因%arg2 效果，  【%arg】 对 %from 的效果被抵消。",
	
	["alliance_feast"] = "联军盛宴",
	[":alliance_feast"] = "锦囊牌·全体性的普通锦囊牌<br />使用时机：出牌阶段。<br />使用目标：所有角色。<br />作用效果：目标角色暗置其所有明置手牌，然后重置并重置装备区里的所有横牌，最后摸<font color=\"green\"><b>一张</b></font>牌。◆若一名角色的人物牌处于连环状态或其装备区里有横牌或其手牌里有明牌，则该角色处于异常状态。<br />◆对不处于异常状态的角色无效。",

	["fight_together"] = "勠力同心",
	[":fight_together"] = "锦囊牌·非全体性的普通锦囊牌<br />使用时机：出牌阶段。<br />使用目标：距离1以内的至少一名角色。<br />作用效果：你选择一项：\n1.明置目标角色的<font color=\"green\"><b>一张</b></font>手牌。   \n2.横置目标角色装备区里的<font color=\"green\"><b>一张</b></font>牌 。   \n3.令目标角色横置。 <br />执行动作：此回合结束时，若成为过此牌目标的角色可以从你未选择过的一项效果执行相反操作的效果，则其须选择其中一项并执行对应的相反操作的效果（明置手牌对应暗置手牌、横置装备区里的牌对应重置装备区里的横牌、令目标角色横置对应令目标角色重置）。",
	["fight_together_effect"] = "勠力同心(后续)",
	["#FightTogetherEffect"] = "%from 执行 勠力同心(后续)",
	
	["bone_healing"] = "刮骨疗毒",
	[":bone_healing"] = "锦囊牌·非全体性的普通锦囊牌<br />使用时机：出牌阶段。<br />使用目标：一名处于异常状态的其他角色。<br />作用效果：你对目标角色造成<font color=\"green\"><b>1点</b></font>伤害，若如此做，其暗置所有手牌，然后重置并重置装备区里的所有牌。◆若一名角色的人物牌处于连环状态或其装备区里有横牌或其手牌里有明牌，则该角色处于异常状态。",
	
}
