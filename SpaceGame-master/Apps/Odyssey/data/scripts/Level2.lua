playerStartX = 128
playerStartY = 128
 
 

 
 botTypes = {
 
 SKULL_BOT = {bot1={x=288,y=2080},bot2={x=308,y=2080},bot3={x=328,y=2080},
			  bot4={x=576,y=2880},bot5={x=1056,y=3072},bot6={x=960,y=2112},
			  bot7={x=832,y=2144},bot8={x=2880,y=1664},bot9={x=2880,y=1216},
			  bot10={x=2880,y=700},bot11={x=2880,y=864},bot12={x=2880,y=480},
			  bot13={x=864,y=416},bot14={x=1064,y=416},bot15={x=1264,y=416},
			  bot16={x=1364,y=416},bot17={x=964,y=416},bot18={x=1164,y=416}},
			  
SPIDER_BOT = {bot1={x=1056,y=2240},bot2={x=480,y=1600},bot3={x=480,y=1300},
				bot4={x=480,y=1300}, bot5={x=1184,y=2112}, bot6={x=1300,y=2112},
				bot7={x=2272,y=2080}, bot8={x=2272,y=2280}, bot9={x=2472,y=2080}},
				
TIGER_BOT = {bot1={x=864, y=160},bot2={x=1064, y=160}, bot3={x=2336,y=416},
			  bot4={x=2336,y=616}},

MAGE_BOSS = {bot1={x=2688,y=2628}}				
 --RANGE_BOT = {bot1={x=40,y=50},bot2={x=140,y=150}}
 
 }


function getBotType(type)
return type
end

function getBotX(x)
return x
end

function getBotY(y)
return y
end
 
 function processBot(type,y,x)
 	--print(type)
 	--print(y)
 	--print(x)
	getBotType(type)
	getBotX(y)
	getBotY(x)
 	end
	
	function getAllBots()
 for k, v in pairs(botTypes) do 
 	--print(k) 
 	for t,z in pairs(v) do
 		--print(t)
 
 			processBot(k,z.x,z.y)
 		end
 end
 end