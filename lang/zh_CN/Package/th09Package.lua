return
{    
    ["th09"] = "战势力",
    ["zhan"] = "战",
    
    --zhan001  势力：战 3血
    ["zhan001"] = "四季映姬·亚玛萨那度" ,
    ["&zhan001"] = "四季映姬" ,
    ["#zhan001"] = "乐园的最高裁判长" ,
    ["!zhan001"] = "四季映姬" ,
    ["shenpan"] = "审判" ,
    [":shenpan"] = "摸牌阶段，你可以放弃摸牌，改为对一名其他角色造成1点雷电伤害。此阶段结束时，若该角色的手牌数大于其体力值，你摸一张牌。" ,
    ["@shenpan-select"] = "你可以对一名其他角色发动“审判”。" ,
        
    ["huiwu"] = "悔悟" ,
    [":huiwu"] = "当你成为其他角色使用的【杀】或非延时类锦囊牌的目标时，该角色可令此牌对你无效并令你摸一张牌。" ,
    ["huiwu:target"] = "你使用【%dest】时，指定了<font color=\"#00FF00\"><b>%src </b></font>为目标，你可以对<font color=\"#00FF00\"><b>%src </b></font>发动“悔悟”。" ,
        
    ["huazhong"] = "花冢" ,
    [":huazhong"] = "<font color=\"orange\"><b>主公技，</b></font>每当其他战势力角色造成1点伤害后，该角色可以判定，若结果为红色，你获得此判定牌。" ,
    ["@huazhong-select"]="你可以指定拥有主公技“花冢”的角色，你判定，若结果为红色，该角色获得此判定牌。",
    
    --zhan002  势力：战 4血
    ["zhan002"] = "小野塚小町",
    ["#zhan002"] = "三途河的领路人",
    ["!zhan002"] = "小野塚小町",
    ["mingtu"]="冥途",
    [":mingtu"]="当一名角色进入濒死状态前，你可以摸一张牌。",
        
    ["silian"] = "死镰",
    [":silian"] = "<font color=\"blue\"><b>锁定技，</b></font>每当你使用【杀】对目标角色造成伤害时，若其体力值为1，此伤害+2。",
    ["#silian"] = "%from 的技能“死镰”被触发, %from 使用的【杀】对 %to 造成的伤害+2。",
    
    --zhan003  势力：战 4血
    ["zhan003"] = "风见幽香",
    ["#zhan003"] = "四季的鲜花之主",
    ["!zhan003"] = "风见幽香",
    ["weiya"] = "威压", 
    [":weiya"] = "<font color=\"blue\"><b>锁定技，</b></font>你的回合内，每当其他角色使用或打出一张基本牌或【无懈可击】时（代替判定牌时除外），该角色须选择一项：令此牌无效，或弃置一张与此牌名称相同的牌。",
    ["@weiya"]="由于“威压”的效果，你需要弃置一张【%src】，否则之前的【%src】无效。",
    ["#weiya"]="由于“%arg”的效果，%from的【%arg2】无效。",
    ["#weiya_ask"]="由于“%arg”的效果，%from 需要弃置一张【%arg2】。",
    
    --zhan004  势力：战 3血
    ["zhan004"]="梅蒂馨·梅兰克莉",
    ["&zhan004"]="梅蒂馨",
    ["#zhan004"]="小小的甜蜜毒药",
    ["!zhan004"]="梅蒂馨",
    ["judu"]="剧毒",
    [":judu"]="每当你对其他角色造成伤害后，你可以判定，若结果为黑色2-9，该角色失去1点体力。",
        
    ["henyi"]="恨意",
    [":henyi"]="一名角色的出牌阶段结束时，若此回合内你受到过伤害，你可以选择：视为你使用一张【万箭齐发】。当此【万箭齐发】对当前回合的角色造成伤害时，你回复1点体力。",

	
    --zhan005  势力：战 4血
    ["zhan005"]="记者文",
    ["#zhan005"]="捏造新闻记者",
    ["!zhan005"]="SP记者文",
    ["toupai"]="偷拍",
    [":toupai"]="摸牌阶段，你可以放弃摸牌，改为观看一名有手牌的其他角色的手牌，你可以依次弃置其中的一至三张红色牌。每以此法弃置一张基本牌，你摸一张牌。",
    ["@toupai-select"]="你可以放弃摸牌，指定一名其他角色，观看其所有手牌，你可以弃置其中的一至三张红色牌。每以此法弃置一张基本牌，你摸一张牌。",
    
    --zhan006  势力：战 3血
    ["zhan006"] = "伊吹萃香", 
    ["#zhan006"] = "不羁奔放的古豪",
    ["!zhan006"] = "伊吹萃香",
    ["zuiyue"] = "醉月",
    [":zuiyue"] = "出牌阶段，当你使用一张锦囊牌时，你可以选择：视为你使用一张【酒】。",
        
    ["doujiu"] = "斗酒",
    [":doujiu"] = "其他角色的出牌阶段，每当该角色对该角色使用一张【桃】或【酒】时，若该角色体力值不小于1，你可以摸一张牌并与该角色拼点，若你赢，此牌对该角色无效，你回复1点体力，结束当前阶段。",
        
    ["yanhui"] = "宴会",
    [":yanhui"] = "<font color=\"orange\"><b>主公技，</b></font>其他战势力角色可于其出牌阶段对你使用【桃】，可当你处于濒死状态并向其求【桃】时对你使用【酒】。",
    ["yanhuivs"] = "宴会给牌",
    [":yanhuivs"] = "若你属于战势力，你可于出牌阶段对拥有主公技“宴会”的角色使用【桃】。当拥有主公技“宴会”的角色处于濒死状态并向你求【桃】时，你可以对其使用【酒】。",
    
    --zhan007  势力：战 4血
    ["zhan007"] = "比那名居天子",
    ["#zhan007"] = "非想非非想天的少女",
    ["!zhan007"] = "比那名居天子",
    ["feixiang"] = "绯想",
    [":feixiang"] = "当判定牌生效前，你可以展示一名角色的一张手牌并令该角色打出此牌代替判定牌。",
    ["@feixiang-playerchosen"] = "<font color=\"#00FF00\"><b>%src </b></font> 将要进行 “%dest” 的判定，你可以发动“绯想”，指定一名角色并展示其一张手牌。",
        
        
    ["dizhen"] = "地震",
    [":dizhen"] = "当一名角色的红色判定牌生效后，若该角色的体力值不小于1，你可以对其造成1点伤害。",
    ["dizhen:target"] = "<font color=\"#00FF00\"><b>%src </b></font> 的 “%dest” 的判定结果为红色，你可以对其发动“地震”。",
        
    ["tianren"] = "天人",
    [":tianren"] = "<font color=\"orange\"><b>主公技，</b></font>你的回合外，每当你需要使用或打出一张【杀】或【闪】时，你可令其他战势力角色依次选择是否代替你打出一张【杀】或【闪】。",
    ["@tianren-slash"]="请打出一张【杀】响应 %src “天人”",
    ["@tianren-jink"] = "请打出一张【闪】响应 %src “天人”",
        
    --zhan008  势力：战 4血    
    ["zhan008"] = "永江衣玖",
    ["#zhan008"] = "美丽的绯之衣",
    ["!zhan008"] = "永江衣玖",
    ["jingdian"] = "静电",
    [":jingdian"] = "<font color=\"blue\"><b>锁定技，</b></font>每当你受到1点雷电伤害时，防止此伤害，你摸三张牌。",
    ["#jingdian"] = "%from 的技能 “%arg” 被触发，%from 防止了 %arg2 点雷电伤害。",
        
    ["leiyun"] = "雷云",
    [":leiyun"] = "你可以将一张<font size=\"5\", color=\"#808080\"><b>♠</b></font>或<font size=\"5\", color=\"#FF0000\"><b>♥</b></font>手牌当【闪电】使用。",

    --zhan009  势力：战 4血
    ["zhan009"] = "姬海棠果",
    ["#zhan009"] = "当代的念写记者",
    ["!zhan009"] = "姬海棠果",
    ["kuaizhao"] = "快照",
    [":kuaizhao"] = "摸牌阶段摸牌时，你可以少摸一张牌，并令你攻击范围内的一名有手牌的角色展示其所有手牌，你摸X张牌（X为其中基本牌的数量且最多为2）。",
    ["@kuaizhao-select_one"] = "指定一名其他角色，展示其所有手牌。",
         
    ["duanjiao"] = "短焦",
    [":duanjiao"] = "<font color=\"blue\"><b>锁定技，</b></font>你的攻击范围为3。",
   
    ["zhan010"] = "秦心",
    ["#zhan010"] = "表情丰富的扑克脸",
    ["!zhan010"] = "秦心",

}


  