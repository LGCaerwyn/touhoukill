return{
    ["touhougod"]="神",
    --["touhougods"]="神(东方包)",
    ["#ChooseKingdom"]="%from 选择了 %arg 势力",

  ["shen000"] = "ZUN",["#shen000"] = "创幻神主",
  ["!shen000"] = "ZUN",
["chuanghuan"] = "创幻",
        [":chuanghuan"] = "<font color=\"blue\"><b>(粗略完成，有待大修改。)</b></font><font color=\"#808080\"><b>永久技，</b></font>所有玩家均展示人物牌后，你随机获得人物牌堆里的四张人物牌，称为“幻想牌”。准备阶段开始时，你随机获得两张“幻想牌”。你可以在恰当的时机，将一张“幻想牌”置入人物牌堆，获得该“幻想牌”的一个技能（永久技、限定技、觉醒技、主公技除外），并在下列时机时失去该技能：1获得该技能的回合结束时 2 此技能发动或被触发，并执行相应效果后（回合内）。",
       ["@huanxiangs"]="幻想牌",
       
       ["shenzhu"] = "神主",
        [":shenzhu"] = "<font color=\"#808080\"><b>永久技，</b></font>你的体力上限为玩家数的一半（向上取整）。",
        
        ["chuanghuan_browse"]="查看幻想牌",
        ["chuanghuan_select"]="获得技能",
        ["#chuanghuangain"]="%from获得了“%arg”",
        ["#chuanghuan1"]="%from发动了“%arg”",
    
    --shen001  势力：神 4血
    ["shen001"] = "神 八云紫",
    ["&shen001"] = "神八云紫",
    ["#shen001"] = "幻想之界",
    ["!shen001"] = "神八云紫",  
    ["jiexian"] = "界线",
    [":jiexian"] = "每当一名角色受到伤害时，你可以弃置一张<font size=\"5\", color=\"#FF0000\"><b>♥</b></font>牌，防止此伤害，并令该角色回复1点体力。每当一名角色回复体力时，你可以弃置一张<font size=\"5\", color=\"#808080\"><b>♠</b></font>牌，防止此体力回复效果，并令该角色受到无来源的1点伤害。",
    ["@jiexiandamage"] = "你可以发动“界线”，弃置一张<font size=\"5\", color=\"#FF0000\"><b>♥</b></font>牌，防止 <font color=\"#00FF00\"><b>%src </b></font> 受到的伤害，并使其回复1点体力。",
    ["@jiexianrecover"] = "你可以发动“界线”，弃置一张<font size=\"5\", color=\"#808080\"><b>♠</b></font>牌，防止 <font color=\"#00FF00\"><b>%src </b></font> 的体力回复效果，并令其受到无来源的1点伤害。",
    ["#jiexianrecover"]="由于“%arg”的效果，%from 受到的 %arg2 点体力回复效果被防止。",
    ["#jiexiandamage"]="由于“%arg”的效果，%from 受到的 %arg2 点伤害被防止。",

    --shen002  势力：神 3血
    ["shen002"] = "神 蕾米莉亚·斯卡蕾特",
    ["&shen002"] = "神蕾米莉亚",
    ["#shen002"] = "永远的红之幼月",
    ["!shen002"] = "神蕾米莉亚",
    ["zhouye"] = "昼夜",
    [":zhouye"] = "<font color=\"blue\"><b>锁定技，</b></font>准备阶段开始时，你须弃所有“夜”标记，然后将牌堆顶的一张牌置入弃牌堆，若为黑色，你获得一枚“夜”标记。若你没有“夜”标记，你不能使用【杀】。",
    ["@ye"]="夜",
 
    ["hongwu"] = "红雾",
    [":hongwu"] = "出牌阶段，若你没有“夜”标记，你可以弃置两张红色牌，获得一枚“夜”标记。",

    ["shenqiang"] = "神枪",
    [":shenqiang"] = "出牌阶段，若你有“夜”标记，你可以弃置一张<font size=\"5\", color=\"#FF0000\"><b>♥</b></font>牌或武器牌，对一名其他角色造成1点伤害。",

    ["yewang"] = "夜王",
    [":yewang"] = "<font color=\"blue\"><b>锁定技，</b></font>每当你受到伤害时，若你有“夜”标记，此伤害-1。",
    ["#YewangTrigger"]="%from 的 “%arg” 被触发，减少了 %arg2 点伤害。",
     
    --shen003  势力：神 4血
    ["shen003"] = "神 琪露诺",
    ["&shen003"] = "神琪露诺",
    ["#shen003"] = "最强",
    ["!shen003"] = "神琪露诺", 
    ["aoyi"] = "奥义",
    ["#aoyi"] = "奥义",  
    [":aoyi"] = "<font color=\"blue\"><b>锁定技，</b></font>你不能使用非延时类锦囊牌。你的非延时类锦囊牌均视为冰【杀】。你使用冰【杀】时，此【杀】不计入每阶段的使用限制。每当你使用一张冰【杀】时，你须选择一项：摸X张牌（X为你已损失的体力值），或弃置场上的一张牌。", 
    ["aoyi1"] = "摸X张牌（X为你已损失的体力值）",
    ["aoyi2"] = "弃置场上的一张牌",
    ["aoyi_chosenplayer"]="请选择“奥义”弃牌的目标。",
    ["ice_nature"] = "冰属性",
    ["ice_slash"] = "冰杀",
 
    --shen004  势力：神 4血
    ["shen004"] = "神 灵乌路空",
    ["&shen004"] = "神灵乌路空", 
    ["#shen004"] = "地底的太阳",
    ["!shen004"] = "神灵乌路空",  
    ["shikong"] = "失控", 
    [":shikong"] = "<font color=\"blue\"><b>锁定技，</b></font>出牌阶段，你使用【杀】时可以且须指定你攻击范围内的所有角色为目标。", 
 
    ["ronghui"] = "熔毁", 
    [":ronghui"] = "<font color=\"blue\"><b>锁定技，</b></font>出牌阶段，每当你使用【杀】对目标角色造成伤害时，你须弃置其装备区里的所有牌。", 

    ["jubian"] = "聚变", 
    [":jubian"] = "<font color=\"blue\"><b>锁定技，</b></font>出牌阶段，在你使用的牌结算后，若此牌对不少于两名角色造成过伤害，你须回复1点体力。",

    ["hengxing"] = "恒星", 
    [":hengxing"] = "<font color=\"blue\"><b>锁定技，</b></font>结束阶段开始时，你须失去1点体力，然后摸三张牌。", 

    --shen005  势力：神 0血
    ["shen005"] = "神 伊吹萃香",
    ["&shen005"] = "神伊吹萃香", 
    ["#shen005"] = "虚幻的萃聚之梦",
    ["!shen005"] = "神伊吹萃香",
    ["huanmeng"] = "幻梦", 
    [":huanmeng"] = "<font color=\"#808080\"><b>永久技，</b></font>你没有体力和体力上限。你须跳过摸牌阶段和弃牌阶段。你的回合开始时，若你没有手牌，你死亡（无来源）。", 
 
    ["cuixiang"] = "萃想", 
    [":cuixiang"] = "<font color=\"blue\"><b>锁定技，</b></font>准备阶段开始时，所有其他角色须依次弃置一张手牌（若没有手牌，将牌堆顶的一张牌置入弃牌堆），然后你获得零至两张以此法置入弃牌堆的牌。", 
    ["cuixiang-exchange"] ="<font color=\"#00FF00\"><b>%src </b></font> 的“%dest”被触发，请弃置一张手牌。",
        
    ["xuying"] = "虚影", 
    [":xuying"] = "<font color=\"#808080\"><b>永久技，</b></font>每当你抵消一张【杀】时，你须摸一张牌。每当你未能抵消一张【杀】时，若你有手牌，你须弃置一半的手牌（向下取整且最少为一）。", 
    ["xuying_discard"]="“虚影”：你未能抵消【杀】，请弃置 %src 张手牌。",
    
    --shen006  势力：神 3血
    ["shen006"]="神 芙兰朵露·斯卡雷特",
    ["&shen006"]="神芙兰朵露",
    ["#shen006"]="绯色月下",
    ["!shen006"]="神芙兰朵露",
    ["kuangyan"]="狂宴",
    [":kuangyan"]="其他角色的回合内，当你进入濒死状态时，你可以获得一枚“禁忌”标记并回复体力至1，然后对当前回合的角色造成1点伤害。",
    ["kuangyan:recover"]="你可以发动“狂宴”，获得一枚“禁忌”标记并回复体力至1，然后对当前回合的角色 <font color=\"#00FF00\"><b> %src </b></font> 造成1点伤害。",
    
    ["huimie"]="毁灭",
    [":huimie"]="<font color=\"green\"><b>出牌阶段限一次，</b></font>你可以获得一枚“禁忌”标记并将一名其他角色的人物牌横置，然后对该角色造成1点火焰伤害。",
    
    ["jinguo"]="禁果",
    [":jinguo"]="<font color=\"blue\"><b>锁定技，</b></font>出牌阶段结束时，你须判定，若结果不为<font size=\"5\", color=\"#808080\"><b>♠</b></font>，你须选择一项：弃置X张牌（X为“禁忌”标记的数量），或失去X/2点体力（向下取整）。",
    ["@kinki"]="禁忌",
    ["@jinguo"]="“禁果”：请弃置 %src 张牌，否则将失去 %dest 点体力。",
    
    --shen007  势力：神 3血
    ["shen007"] = "神 十六夜咲夜",
    ["&shen007"] = "神十六夜咲夜",
    ["#shen007"] = "铭刻的月之钟",
    ["!shen007"] = "神十六夜咲夜",
    ["shicao"] = "时操",
    [":shicao"] = "<font color=\"blue\"><b>锁定技，</b></font>准备阶段开始时，若此回合不是额外回合，你获得一枚“时”标记。",
    ["@clock"]="时",

    ["shiting"] = "时停",
    [":shiting"] = "一名角色的回合（额外回合除外）开始前，若你有“时”标记，你可以弃所有“时”标记，进行一个额外的回合。",
    ["shiting:extraturn"] ="在 <font color=\"#00FF00\"><b>%src </b></font> 的回合开始前,你可以其发动“时停”，进行一个额外的回合。",

    ["huanzai"] = "幻在",
    [":huanzai"] = "<font color=\"red\"><b>限定技，</b></font>结束阶段开始时，你可以获得一枚“时”标记。",
    ["@huanzai"]="幻在",
    ["$huanzaiAnimate"]="image=image/animate/huanzai.png",

    ["shanghun"] = "伤魂",
    [":shanghun"] = "<font color=\"red\"><b>限定技，</b></font>当你受到伤害后，你可以获得一枚“时”标记。",
    ["$shanghunAnimate"]="image=image/animate/shanghun.png",
    ["@shanghun"]="伤魂",
    ["#touhouExtraTurn"]="%from 将进行一个额外的回合。",
 
    --shen008  势力：神 3血+3血
    ["shen008"] = "神 魂魄妖梦", 
    ["&shen008"] = "神魂魄妖梦",
    ["#shen008"] = "半生半死、半悟半惘",
    ["!shen008"] = "神魂魄妖梦",
    ["banling"] = "半灵",
    [":banling"] = "<font color=\"#808080\"><b>永久技，</b></font>你拥有两种体力：人体力和灵体力。你的基础手牌上限与两种体力中较少的那种相同。每当你的体力或体力上限发生1点改变时，你选择两种体力中的一种进行结算。当任一种体力值扣减至0时，你进入濒死状态。",
    ["#lingtilidamage"] = "%from 的 %arg 被触发， %from 选择了扣减 %arg2 点 “灵”体力。",
    ["#rentilidamage"] = "%from 的 %arg 被触发， %from 选择了扣减 %arg2 点 “人”体力。",
    ["#rentilirecover"] = "%from 的 %arg 被触发， %from 选择了回复 %arg2 点 “人”体力。",
    ["#lingtilirecover"] = "%from 的 %arg 被触发， %from 选择了回复 %arg2 点 “灵”体力。",
    ["banling_minus:rentili"] = "人体力(右)",
    ["banling_minus:lingtili"] = "灵体力(左)" ,
    ["banling_plus:rentili"] = "人体力(右)",
    ["banling_plus:lingtili"] = "灵体力(左)" ,
    ["@lingtili"] = "灵体力",
    ["lingtili"] = "灵体力",
    ["@lingtili"] = "灵体力",
    ["rentili"] = "人体力",
    ["@rentili"] = "人体力",
    ["banling_minus"]="半灵 扣减体力",
    ["banling_plus"]="半灵 回复体力",
        
    ["rengui"] = "人鬼",
    ["renguidraw"] = "人鬼(摸牌)",
    ["renguidiscard"] = "人鬼(弃牌)",
    [":rengui"] = "准备阶段开始时，你可令一名角色摸X张牌（X为你已损失的灵体力值且最多为2），然后你可以弃置一名角色的Y张牌（Y为你已损失的人体力值且最多为2）。",
    ["@rengui-draw"] = "你可令一名角色摸 %src 张牌。",
    ["~rengui"] = "指定一名角色→点击“确定”。",
    ["@rengui-discard"] = "你可以弃置一名角色的 %src 张牌。" ,
    
    --shen009  势力：神 4血
    ["shen009"] = "神 铃仙·优昙华院·稻叶",
    ["&shen009"] = "神铃仙",
    ["#shen009"] = "狂气的赤眼",
    ["!shen009"] = "神铃仙",      
    ["ningshi"] = "凝视", 
    [":ningshi"] = "<font color=\"blue\"><b>锁定技，</b></font>出牌阶段，当你使用【杀】或锦囊牌指定其他角色为目标后，若此牌只指定了一名角色为目标，该角色须失去1点体力。",
    ["@ningshi"] ="请弃置 %src 张牌，否则将失去 %src 点体力",
  
    ["gaoao"] = "高傲",
    [":gaoao"] = "<font color=\"blue\"><b>锁定技，</b></font>你的回合外，若一张牌将进入你的区域，改为将其置入弃牌堆。", 
    
    --shen010  势力：神 4血
    ["shen010"] = "神 东风谷早苗",
    ["&shen010"] = "神东风谷早苗",
    ["#shen010"] = "现代人的现人神",
    ["!shen010"] = "神东风谷早苗",     
    ["shenshou"] = "神授", 
    [":shenshou"] = "<font color=\"green\"><b>出牌阶段限一次，</b></font>你可以展示一张手牌并将其交给一名其他角色。若此牌为你最后的手牌，此牌视为【杀】，花色视为<font size=\"5\", color=\"#808080\"><b>♠</b></font>，点数视为5。<br /> 1. 若此牌为【杀】，你可以选择：视为该角色对其攻击范围内的由你指定的一名角色使用一张【杀】。 <br />2. 若此牌为<font size=\"5\", color=\"#808080\"><b>♠</b></font>牌，你可令该角色获得其攻击范围内的由你指定的一名角色的一张手牌。 <br />3. 若此牌的点数为5-9，你可以摸一张牌。", 
    ["shenshou_slash"]="视为该角色对另一名角色使用一张【杀】",
    ["@shenshou-slash"]="请指定一名角色，视为 <font color=\"yellow\"><b>%src </b></font> 对其使用一张【杀】。",
    ["shenshou_obtain"]="令该角色获得另一名角色的一张手牌",
    ["@shenshou-obtain"]="请指定一名角色，<font color=\"yellow\"><b>%src </b></font> 获得其一张手牌。",
    ["shenshou_draw"]="你摸一张牌",
    ["$ShenshouTurnOver"]="%from 发动 %arg 展示了最后的手牌，此牌被视为 %card。",
    
    --shen011  势力：神 4血
    ["shen011"] = "神 博丽灵梦",
    ["&shen011"] = "神博丽灵梦", 
    ["#shen011"] = "大结界的守护者",
    ["!shen011"] = "神博丽灵梦",
 --[[["jiejie"] = "结界", 
 [":jiejie"] = "出牌阶段，你可以弃置一张手牌，将牌堆顶的两张牌分别置于一名角色和其上、下家之间，称为“结界”。每阶段限一次。两名角色间可以放置任意数量的“结界”。“结界”在计算距离时占据无穷多个位置。一名角色的出牌阶段，该角色可以弃置一张【杀】，将其攻击范围内的一张“结界”置入弃牌堆。", 
 ["jiejie_confirm"]="指定结界",
 ["@jiejie"]="请选择指定结界时要使用的【杀】",
 ["jiejie_chosenplayer"]="请选择攻击范围内的结界牌（你可以指定额外目标时会被多次询问）",
 ["jiejie_chosenslashtarget"]="请继续指定实际要杀的目标（你可以指定额外目标时会被多次询问）",
 ["jiejie_left"]="左结界",
["jiejie_right"]="右结界",
["@in_jiejie"]="结界中",
 [":jiejie_confirm"]="你可以对“结界”出杀。",
 
 ["reimu_fengyin"] = "封印", 
 [":reimu_fengyin"] = "摸牌阶段摸牌后，你可以弃置一张<font size=\"5\", color=\"#FF0000\"><b>♥</b></font>手牌，对两张距离为2的“结界”间的一名其他角色造成1点伤害，然后将其人物牌翻面，将此两张“结界”置入弃牌堆。 ",  
["@fengyin-discard"]="你可以弃置一张<font size=\"5\", color=\"#FF0000\"><b>♥</b></font>手牌，发动【结界】，对“结界”中的一名其他角色造成1点伤害，并令其翻面。",
["fengyin_chosenplayer"]="选择一名身处结界中的封印目标",]]
    
    --shen012  势力：神 4血
    ["shen012"] = "神 四季映姬·亚玛萨那度" ,
    ["&shen012"] = "神四季映姬" ,
    ["#shen012"] = "幻想乡的裁判长" ,
    ["!shen012"] = "神四季映姬" ,
    ["quanjie"] = "劝诫" ,
    [":quanjie"] = "其他角色的出牌阶段开始时，你可令其选择一项：摸一张牌，不能使用【杀】，直到回合结束；或弃置一张【杀】。" ,
    ["@quanjie-discard"] = "请弃置一张【杀】，否则你摸一张牌并不能使用【杀】直到回合结束。" ,
        
    ["@duanzui-extra"]="断罪",
    ["duanzui"] = "断罪" ,
    [":duanzui"] = "其他角色的回合结束后，若此回合内有角色死亡，你可以将你的人物牌翻至正面朝上，进行一个额外的回合，并获得技能“审判”（摸牌阶段，你可以放弃摸牌，改为对一名其他角色造成1点雷电伤害。此阶段结束时，若该角色的手牌数大于其体力值，你摸一张牌），直到此额外回合结束。" ,
    
    --shen013  势力：神 3血
    ["shen013"] = "神 红美铃", 
    ["&shen013"] = "神红美铃",
    ["#shen013"] = "龙神的化身",
    ["!shen013"] = "神红美铃",
	["huaxiang"] = "华想",
	[":huaxiang"] = "每当你需要使用或打出一张基本牌或【无懈可击】时，你可以声明之（若你的体力上限大于3，不能声明【闪】；若你的体力上限大于2，不能声明【桃】；若你的体力上限大于1，不能声明【无懈可击】），并将一张与你人物牌上的任何一张牌花色均不同的手牌置于你的人物牌上，称为“虹”，视为你使用或打出了一张你声明的牌。",
    ["huaxiang_skill_slash"]= "华想";
    ["huaxiang_skill_saveself"]= "华想";
	["rainbow"] = "虹",
	
	["caiyu"] = "彩雨",
	[":caiyu"] = "一名角色的结束阶段开始时，若你的人物牌上有不少于四张“虹”，你可以将所有“虹”置入弃牌堆，摸两张牌，然后你可以失去1点体力上限。",
	["caiyu:discard"] ="你可以其发动“彩雨”，弃置所有“虹”，并摸两牌。",
	["caiyu:loseMaxHp"] ="你是否执行“彩雨”效果，扣减1点体力上限。",
	
	["xuanlan"] = "绚烂",
	[":xuanlan"] = "若你未受伤，你可以跳过弃牌阶段。 ",
    
	
	--["hualong"] = "化龙",
    --[":hualong"] = "<font color=\"purple\"><b>觉醒技，</b></font>当你进入濒死状态时，你须增加1点体力上限，然后回复体力至上限，并改变技能“乱舞”“龙威”。",
    --["$hualongAnimate"]="image=image/animate/hualong.png",
        
    --["luanwu"] = "乱舞",
    --[":luanwu"] = "你可以将两张颜色相同的手牌（技能“化龙”发动后，改为一张牌）当【杀】或【闪】使用或打出。",

    --["longwei"] = "龙威",
    --[":longwei"] = "当你成为其他角色使用的【杀】或锦囊牌的目标时，你可令该角色弃置一张牌（技能“化龙”发动后，改为你依次弃置该角色的两张牌）。",
    --["@longwei-askfordiscard"]="<font color=\"#00FF00\"><b>%src </b></font> 发动了“龙威”，请弃置 %dest 张牌。",
    
	
	
    --shen014  势力：神 4血
    ["shen014"] = "神 八意永琳",
    ["&shen014"] = "神八意永琳",
    ["#shen014"] = "月都大贤者",
    ["!shen014"] = "神八意永琳",
    ["qiannian"] = "千年",
    [":qiannian"] = "<font color=\"blue\"><b>锁定技，</b></font>游戏开始时或每进行一次洗牌后，你须获得一枚“岁月”标记。摸牌阶段摸牌时，你须多摸X张牌。你的手牌上限增加X的两倍（X为“岁月”标记的数量）。",
    ["@qiannian"]="岁月",
    
    --shen015  势力：神 4血
    ["shen015"] = "神 八坂神奈子" ,
    ["&shen015"] = "神八坂神奈子",
    ["#shen015"] = "大和神" ,
    ["!shen015"] = "神八坂神奈子",   
    ["qinlue"] = "侵略" ,
    [":qinlue"] = "其他角色的出牌阶段开始前（额外回合内除外），你可以弃置一张【杀】或装备牌，令该角色选择一项：弃置一张【闪】；或跳过此出牌阶段并令你进行一个额外的出牌阶段。此额外阶段开始时，你须将所有手牌面朝下置于你的人物牌上，然后获得该角色的所有手牌。此额外阶段结束后，你须将所有手牌交给该角色，然后将你人物牌上的所有牌加入手牌。",
    ["@qinlue-discard"]="你可以对 <font color=\"#00FF00\"><b>%src </b></font> 发动“侵略”。";
    ["@qinlue-discard1"]="<font color=\"#00FF00\"><b>%src </b></font> 发动了“侵略”，请弃置一张【闪】，否则其将代替你进行出牌阶段。";
    
    --shen016  势力：神 4血
    ["shen016"] = "神 圣白莲" ,
    ["&shen016"] = "神圣白莲" ,
    ["#shen016"] = "灭除八苦的尼公" ,
    ["!shen016"] = "神圣白莲" ,
    ["chaoren"] = "超人" ,
    [":chaoren"] = "牌堆顶的牌对你可见。你可以使用或打出此牌。",
    ["$chaorendrawpile"] = "牌堆顶的牌为： %card" ,
    
    --shen017  势力：神 3血
    ["shen017"] = "神 古明地恋", 
    ["&shen017"] = "神古明地恋",
    ["#shen017"] = "哈特曼的妖怪少女",
    ["!shen017"] = "神古明地恋",
    ["biaoxiang"] = "表象", 
    [":biaoxiang"] = "<font color=\"purple\"><b>觉醒技，</b></font>准备阶段开始时，若你的手牌数小于二，你须将体力牌翻面，并获得技能“自我”（出牌阶段，你可以弃置两张手牌，回复1点体力）。", 
    ["#BiaoxiangWake"]="%from 的手牌数小于 <font color=\"yellow\"><b>2</b></font>，触发“%arg”觉醒。",
    ["$biaoxiangAnimate"]="image=image/animate/biaoxiang.png",
  
    ["shifang"] = "释放", 
    [":shifang"] = "<font color=\"purple\"><b>觉醒技，</b></font>当你失去装备区里的最后的牌时，你须增加体力上限至4，并失去所有技能（觉醒技除外），然后获得技能“本我”（每当你受到伤害时，若你已受伤，你可以摸X张牌，然后令伤害来源弃置X张牌，X为你已损失的体力值）。", 
    ["#ShifangWake"]="%from 失去装备区里的最后的牌，触发“%arg”觉醒。",
    ["$shifangAnimate"]="image=image/animate/shifang.png",
  
    ["yizhi"] = "抑制", 
    [":yizhi"] = "<font color=\"purple\"><b>觉醒技，</b></font>当你进入濒死状态时，你须回复体力至1，失去体力上限至3，并失去所有技能（觉醒技除外），然后获得技能“超我”（结束阶段开始时，你可以弃置一张手牌，令一名体力上限不小于你的角色摸两张牌，若该角色的体力上限为3，你摸两张牌）。", 
    ["#YizhiWake"]="%from 进入濒死状态，触发“%arg”觉醒。",
    ["$yizhiAnimate"]="image=image/animate/yizhi.png",
  
    ["ziwo"] = "自我",
    [":ziwo"] ="出牌阶段，你可以弃置两张手牌，回复1点体力。",
 
    ["benwo"] = "本我",
    [":benwo"] ="每当你受到伤害时，若你已受伤，你可以摸X张牌，然后令伤害来源弃置X张牌，X为你已损失的体力值。",
    ["benwo:invoke"] = "你将受到来自 <font color=\"#00FF00\"><b>%src </b></font> 的伤害，你可以发动“本我”，摸 %dest 张牌，然后令 <font color=\"#00FF00\"><b>%src </b></font> 弃置 %dest 张牌。",
 
    ["chaowo"] = "超我",
    [":chaowo"] ="结束阶段开始时，你可以弃置一张手牌，令一名体力上限不小于你的角色摸两张牌，若该角色的体力上限为3，你摸两张牌。",
    ["@chaowo"] ="你可以发动“超我”，令一名体力上限不小于你的角色摸两张牌，若该角色的体力上限为3，你摸两张牌。",
    ["~chaowo"] ="请选择要弃置的手牌并指定“超我”的目标。",
    ["@chaowo_chosenplayer"] = "请指定一名角色，令其摸两张牌。",

    --shen018  势力：神 5血
    ["shen018"] = "神 泄矢诹访子",
    ["#shen018"] = "名存实亡的神明",
    ["&shen018"] = "神泄矢诹访子",
    ["!shen018"] = "神泄矢诹访子",
    ["zuosui"] = "作祟",
    [":zuosui"] = "每当你对其他角色造成伤害时，你可以防止此伤害并获得一枚“信仰”标记，令该角色选择一个不大于4的正整数X，然后你选择一项：令该角色摸X张牌，然后该角色失去X点体力；或令该角色将牌弃至X张。",
    ["#zuosuichoice"]="由于“%arg”的效果，%from 选择了一个数字 %arg2 ",
    ["zuosui:losehp"]="流失体力",
    ["zuosui:discard"]="弃牌",

    ["shenhua"] = "神话",
    [":shenhua"] = "<font color=\"blue\"><b>锁定技，</b></font>锁定技，结束阶段开始时，若你没有“信仰”标记，你失去1点体力上限；若你有“信仰”标记，弃所有“信仰”标记。",
    
    ["worao"] = "沃饶",
    [":worao"] = "当你成为其他角色使用的【杀】或非延时类锦囊牌的目标时，你可以摸两张牌并获得一枚“信仰”标记，然后将一张手牌交给该角色。",
    ["worao:invoke"]="<font color=\"#00FF00\"><b> %src </b></font>使用【%dest】指定了你为目标，你可以发动“沃饶”。",    
    ["woraoGive"]="请将一张手牌交给“沃饶”对象 <font color=\"#00FF00\"><b>%src </b></font>。",
    ["@xinyang"] = "信仰",

    --shen019  势力：神 4血
     ["shen019"] = "神 丰聪耳神子",
    ["&shen019"] = "神丰聪耳神子",
    ["#shen019"] = "日出之国的天子",
    ["!shen019"] = "神丰聪耳神子",
    ["hongfo"]= "弘佛",
    [":hongfo"]= "摸牌阶段摸牌后，你可以指定势力与你相同的所有角色，这些角色依次摸一张牌。然后若有不少于一名角色的势力与你不同，你须令其中的一名角色将其势力改变为你的势力。",
    ["#hongfoChangeKingdom"] = "%from 被改变为 %arg 势力",
    ["@hongfo"]= "你需要将一名其他角色的势力属性改变为和你相同",
  
    ["junwei"]="君威",
    [":junwei"]="<font color=\"blue\"><b>锁定技，</b></font>当你成为【杀】的目标时，若此【杀】的使用者的势力与你不同，其须选择一项：弃置一张黑色牌，或令此【杀】对你无效。",
    ["@junwei-discard"]="<font color=\"#00FF00\"><b>%src </b></font> 的“君威”被触发，请弃置一张黑色牌，否则【%dest】对其无效。",
  
  
    ["gaizong"]="改宗",
    [":gaizong"]="<font color=\"purple\"><b>觉醒技，</b></font>结束阶段开始时，若势力数不大于2，你须失去1点体力上限，重新选择势力，失去技能“弘佛”，获得技能“问道”（出牌阶段开始时，你可以指定势力各不相同的任意数量的角色，你依次弃置这些角色的一张手牌。每以此法弃置一张红色牌，你回复1点体力）",
    ["#GaizongWake"] = "场上势力数不大于2， %from 触发“%arg”觉醒。",
    ["$gaizongAnimate"]="image=image/animate/gaizong.png",
   
    ["wendao"]="问道",
    [":wendao"]="出牌阶段开始时，你可以指定势力各不相同的任意数量的角色，你依次弃置这些角色的一张手牌。每以此法弃置一张红色牌，你回复1点体力",
    ["@wendao"] = "你可以发动“问道”，指定势力各不相同的任意数量的角色，你依次弃置这些角色的一张手牌。",
    ["~wendao"] = "选择角色，然后确定",
   
   --shen020  势力：神 4血
    ["shen020"] = "神 蓬莱山辉夜",
    ["&shen020"] = "神蓬莱山辉夜",
    ["#shen020"] = "永远的公主",
    ["!shen020"] = "神蓬莱山辉夜",
	["shenbao"] = "神宝",
	[":shenbao"] = "出牌阶段限一次，你可以和一名角色拼点，若你赢，你可以对应两张拼点牌的花色和类型执行下列一项效果：1 红色：回复一点体力; 2黑色：获得场上一张牌;  3 基本牌：视为对其使用一张无视防具的【杀】; 4 锦囊：视为使用一张【无中生有】; 5 装备：令该角色受到一点无来源的火属性伤害。",
	["nanti"] = "难题",
	[":nanti"] = "当你和一名角色拼点前，你可以先放置并展示拼点牌，若如此做，当两张拼点牌的花色或类型相同时，视为你赢。",

 
 
    --shen021  势力：神 4血
    ["shen021"] = "神 小野塚小町", 
    ["&shen021"] = "神小野塚小町", 
    ["#shen021"] = "江户时代气质的死神",
    ["!shen021"] = "神小野塚小町",
    ["yindu"] = "引渡",
    [":yindu"] = "当其他角色死亡时，你可以摸三张牌。若如此做，不执行奖惩。",
    ["yindu:invoke"] = "你是否发动“引渡”，摸 3 张牌，若如此做，<font color=\"#00FF00\"><b>%src </b></font>的死亡将不会执行奖惩",

    ["huanming"] = "换命",
    [":huanming"] = "<font color=\"red\"><b>限定技，</b></font>当你对其他角色造成伤害时，若该角色的体力值不小于1，你可以防止此伤害，并与该角色交换体力。",
    ["$huanmingAnimate"]="image=image/animate/huanming.png",
    
    ["chuanwu"] = "川雾",
    [":chuanwu"] = "<font color=\"blue\"><b>锁定技，</b></font>你与其他角色的距离最多为X（X为该角色的体力值且最少为1）。",
 
 
 
 } 
