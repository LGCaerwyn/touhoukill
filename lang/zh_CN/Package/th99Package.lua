return{
	["th99"] = "外传",
	["wai"] = "外",
	
	
	["wai001"] = "稗田阿求",["#wai001"] = "幻想乡的记忆" ,
      ["!wai001"] = "稗田阿求",
    ["qiuwen"] = "求闻" ,
        [":qiuwen"] = "出牌阶段，你可以摸三张牌。<font color=\"green\"><b>每阶段限一次。</b></font>" ,
     
	 ["zaozu"] = "早卒" ,
        [":zaozu"] = "<font color=\"blue\"><b>锁定技，</b></font>你须跳过弃牌阶段。回合结束阶段开始时，若你的手牌数大于你的体力上限，你须失去1点体力。" ,
	
	["dangjia"] = "当家" ,
		[":dangjia"] ="<font color=\"orange\"><b>主公技，</b></font>其他外势力角色的出牌阶段，若你已受伤，该角色可以与你拼点，若该角色没赢，你回复1点体力。<font color=\"green\"><b>每阶段限一次。</b></font>",
		["dangjiavs"] = "当家拼点" ,
		[":dangjiavs"] = "若你属于外势力，你可以和已受伤的拥有主公技“当家”的角色拼点，若你没赢，该角色回复1点体力。" ,

 ["wai002"] = "森近霖之助",["#wai002"] = "古道具店主" ,
        ["!wai002"] = "森近霖之助",   
	 ["xiufu"] = "修复" ,
        [":xiufu"] = "出牌阶段，你可以将弃牌堆里的一张装备牌置于一名角色的装备区里（可替换装备区里的牌）。<font color=\"green\"><b>每阶段限一次。</b></font>" ,
        ["@xiufu-select"] = "你可以将【%src】置于一名角色的装备区里。",
		
["wai003"] = "朱鹭子",["#wai003"] = "无名的读书妖怪",
["!wai003"] = "朱鹭子",
 ["fandu"] = "泛读", 
 [":fandu"] = "回合开始阶段开始时或每当你受到1点伤害后，你可以摸两张牌，然后令一名其他角色弃置你的一张手牌。", 
 ["@fandu-select"] ="由于“泛读”的效果，请指定一名其他角色，该角色将弃置你的一张手牌。", 
 
 ["taohuanshow:show"] = "<font color=\"#00FF00\"><b>%src </b></font> 获得了你的手牌，你可以发动“讨还”，令其展示之。", 
 ["taohuanshow"]="讨还",
 ["taohuan"] = "讨还", 
 [":taohuan"] = "每当其他角色弃置你的一张非基本牌后或获得你的一张牌后，你可以与该角色拼点，若你赢，你获得该角色的一张牌。",  

  ["wai004"] = "宇佐见莲子",["#wai004"] = "窥知时空之目",
   ["!wai004"] = "宇佐见莲子",
    ["shitu"] = "识途",
        [":shitu"] = "你的回合（额外回合除外）结束后，若此回合内没有角色受到过伤害且没有角色死亡，你可令一名角色进行一个额外的回合，但此回合没有摸牌阶段以外的阶段。",
        ["@shitu-playerchoose"] = "指定一名角色，令其进行一个额外的回合，但此回合没有摸牌阶段以外的阶段。",
		
["wai005"] = "玛艾露贝莉·赫恩",["&wai005"] = "玛艾露贝莉",["#wai005"] = "看穿境界之目",
  ["!wai005"] = "玛艾露贝莉",
 ["mengxian"] = "梦现", 
 [":mengxian"] = "<font color=\"purple\"><b>觉醒技，</b></font>回合开始阶段开始时，若你的人物牌上有不少于三张“境界”，你须失去1点体力上限，摸四张牌，并获得技能“乱影”（每当其他角色使用或打出一张基本牌时（代替判定牌时除外），你可以将一张“境界”交给该角色，并令此基本牌无效）。", 
 ["#MengxianWake"] = "%from 的人物牌上的“境界”为 %arg2 张，触发“%arg”觉醒。",
 ["$mengxianAnimate"]="image=image/animate/mengxian.png",
 
 ["luanying"] = "乱影", 
 [":luanying"] ="每当其他角色使用或打出一张基本牌时（代替判定牌时除外），你可以将一张“境界”交给该角色，并令此基本牌无效。", 

["wai006"] = "绵月丰姬", ["#wai006"] = "连系海与山的月之公主",
   ["!wai006"] = "绵月丰姬",
 ["lianxi"] = "连系",
 [":lianxi"] = "每当你使用或打出一张【杀】时，你可以选择：视为你使用或重铸一张【铁索连环】。", 
 ["@lianxi"]="你可以使用或重铸一张【铁索连环】。",
 ["~lianxi"]="请指定一至两名角色为【铁索连环】的目标，或直接点击“确定”重铸。",
 
 ["yueshi"] = "月使", 
 [":yueshi"] = "<font color=\"purple\"><b>觉醒技，</b></font>回合开始阶段开始时，若你的人物牌横置，你须增加1点体力上限，并获得技能“睿智”（在一张非延时类锦囊牌对你结算后，若你已受伤，你可以进行一次判定，若结果为红色，你回复1点体力）。",  
 ["#YueshiWake"] = "%from 的人物牌横置，触发“%arg”觉醒。",
 ["$yueshiAnimate"]="image=image/animate/yueshi.png",
 
["wai007"] = "绵月依姬", ["#wai007"] = "受神灵凭依的月之公主",
["!wai007"] = "绵月依姬",
 ["pingyi"] = "凭依", 
 --[":pingyi"] = "每当你受到一次伤害后，你可以弃置一张牌，获得伤害来源的一个技能（永久技、限定技、觉醒技、主公技除外），直到伤害来源死亡时或你以此法获得新的技能。若如此做，伤害来源失去你获得的技能，直到你死亡时或当你以此法获得新的技能。",  
 [":pingyi"] ="每当你受到一次伤害后，你可以弃置一张牌，令伤害来源的一个未失效的技能失效（永久技、限定技、觉醒技、主公技除外），直到你死亡时或当你以此法令另一个技能失效。若如此做，你获得此技能，直到伤害来源死亡时或当你以此法令另一个技能失效。",
 ["@pingyi-discard"]="弃置一张牌，“凭依” <font color=\"#00FF00\"><b>%src </b></font> 的技能“<font color=\"#00FF00\"><b>%dest</b></font>”。",
 ["@pingyi"]= "凭依",
 ["pingyi:cancel"] = "不“凭依”任何技能", 
["#pingyiLoseSkill"] = "<font color=\"#00FF00\"><b>%from</b></font> 的技能 “%arg” 因 凭依而失效",
["#pingyiReturnSkill"] = "<font color=\"#00FF00\"><b>%from</b></font> 的技能 “%arg” 重新生效",

 ["wai008"] = "桑尼·米尔克",["&wai008"] = "桑尼",["#wai008"] = "闪耀的日之光",
        ["!wai008"] = "桑尼.米尔克",   
		["zheshe"] = "折射",
        [":zheshe"] = "每当你受到一次伤害时，若伤害来源不是你，你可以弃置一张手牌，防止此伤害，并对一名角色造成1点伤害。伤害结算后，该角色须摸X张牌（X为该角色已损失的体力值）。",
        
		["@zheshe"] = "请弃置一张手牌,发动“折射”，防止此伤害，并对一名角色造成1点伤害。",
        ["~zheshe"] = "你可以弃置一张手牌，防止此伤害，并对一名角色造成1点伤害。",
        
		["tanchi"] = "贪吃",
        [":tanchi"] = "<font color=\"blue\"><b>锁定技，</b></font>每当你回复一次体力时，此体力回复效果+1。",
		
  ["wai009"] = "露娜·琪尔德",["&wai009"] = "露娜",["#wai009"] = "寂静的月之光",
         ["!wai009"] = "露娜·琪尔德",            
		["zhuonong"] = "捉弄",
        [":zhuonong"] = "出牌阶段，你可以选择一项：对一名角色造成1点火焰伤害，然后令其回复1点体力；或令一名角色回复1点体力（可以为未受伤的角色），然后对其造成1点火焰伤害。<font color=\"green\"><b>每阶段限一次。</b></font>",
        ["zhuonong:dr"] = "对其造成1点火焰伤害，然后令其回复1点体力",
        ["zhuonong:rd"] = "令其回复1点体力，然后对其造成1点火焰伤害",
        
		["jijing"] = "寂静",
       [":jijing"] = "<font color=\"blue\"><b>锁定技，</b></font>你的回合内，每当其他角色受到一次伤害后，你与该角色的距离视为1，直到回合结束。",
		["@jijing"] ="寂静",

["wai010"] = "斯塔·莎菲雅",["&wai010"] = "斯塔",["#wai010"] = "照射的星之光",
  ["!wai010"] = "斯塔.莎菲雅",          
 ["ganying"] = "感应", 
 [":ganying"] = "每当你与其他角色的距离或其他角色与你的距离发生一次改变时，你可以摸一张牌，然后你可以弃置该角色的一张手牌。当你与多名其他角色的距离或多名其他角色与你的距离同时发生改变时，你只能发动一次此技能。", 
 ["@ganying"]="选择与你距离改变的其他角色，弃置其一张牌",
 
["wai011"] = "茨木華扇",["#wai011"] = "独臂有角的仙人",["!wai011"] = "茨木華扇",
 ["xingshan"] = "行善",
 [":xingshan"] = "每当一名角色使用一张【杀】或者【决斗】时，你可以令其摸一张牌，此牌无效。每当一名角色在弃牌阶段之外主动弃置了【杀】或【决斗】时，你可以令其摸一张牌。<font color=\"green\"><b>每回合限一次</b></font>",
 ["xingshan:cancel"] =
        "<font color=\"#00FF00\"><b>%src </b></font>使用了【%dest】,你可以其发动“行善”",
["xingshan:discard"] =
        "<font color=\"#00FF00\"><b>%src </b></font>弃置了【%dest】,你可以其发动“行善”",
		
 ["yushou"] = "御兽", 
 [":yushou"] = "出牌阶段，若你的装备区没有坐骑牌，你可以获得弃牌堆中的一张坐骑牌。<font color=\"green\"><b>每阶段限一次。</b></font>", 
 
 
["wai012"] = "本居小铃",["#wai012"] = "识文解意的爱书人", ["!wai012"] = "本居小铃",
["pandu"] = "判读",
[":pandu"] = "出牌阶段，你可以展示一名其他角色的一张手牌：若此牌为【杀】，该角色须对你使用此牌；若此牌不为基本牌，你须获得此牌。<font color=\"green\"><b>每阶段限一次。</b></font>",

["bihuo"] = "避祸",
[":bihuo"] = "当你成为【杀】的目标时，你可以将所有手牌（至少一张）交给一名其他角色（不能是此【杀】的使用者或目标），然后令该角色代替你成为此【杀】的目标。此回合结束时，该角色须将等量的手牌交给你。",
["@bihuo-playerchosen"] = "<font color=\"#00FF00\"><b>%src </b></font> 对你使用了【杀】，你可以发动“避祸”，请指定一名其他角色。",
 ["bihuo_exchange"]="由于“避祸”的效果，请选择 %src 张手牌交给 <font color=\"#00FF00\"><b>%dest </b></font>。",       

 
 ["zulin"] = "租赁",
 [":zulin"] = "每当你使用一张非延时类锦囊结算完毕后，你可以令一名其他角色获得此牌。",
 ["@zulin-obtain"]="你可以发动“租赁”， 选择一名其他角色，令其获得你使用的非延时锦囊 【%src】",
 
 ["shimo"] = "识魔",
 [":shimo"] = "回合外，每当你成为非延时类锦囊的目标时，你可以弃置一张花色相同的手牌，若如此做，本回合内所有非延时类锦囊对你无效。<font color=\"green\"><b>每回合限一次</b></font>",
 ["@shimo-discard1"]="你可以弃置一张<font size=\"5\", color=\"#FF0000\"><b>♠</b></font>手牌，发动“识魔”。本回合内，非延时锦囊对你无效",
	
 ["@shimo-discard2"]="你可以弃置一张<font size=\"5\", color=\"#FF0000\"><b>♥</b></font>手牌，发动“识魔”。本回合内，非延时锦囊对你无效",
["@shimo-discard3"]="你可以弃置一张<font size=\"5\", color=\"#FF0000\"><b>♣</b></font>手牌，发动“识魔”。本回合内，非延时锦囊对你无效",
["@shimo-discard4"]="你可以弃置一张<font size=\"5\", color=\"#FF0000\"><b>♦</b></font>手牌，发动“识魔”。本回合内，非延时锦囊对你无效",
				
 }
