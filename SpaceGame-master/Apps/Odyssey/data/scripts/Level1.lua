playerStartX = 3000
playerStartY = 3000
 
 

 
 botTypes = {
 --m is movement type
 --m catalog
 --0 SQUARE
 --1 CIRCLE - PENDING
 --2 TRIANGLE - PENDING
 --3 LINEAR_HORIZONTAL
 --4 LINEAR_VERTICAL
 --5 IDLE
SKULL_BOT = {bot1={x=360,y=225, m=6},bot2={x=340,y=200, m=0},bot3={x=340,y=700, m=3},
		  bot4={x=360,y=700},bot5={x=380,y=700},bot6={x=400,y=700},
		bot7={x=420,y=700},bot8={x=440,y=700},bot9={x=480,y=700},
		bot10={x=500,y=700},bot11={x=520,y=700,m=5},bot12={x=540,y=700,m=4}},
			  
SPIDER_BOT = {bot1={x=360,y=1000},bot2={x=256,y=1248},bot3={x=500,y=1248},
			  bot4={x=256,y=1504},bot5={x=500,y=1504},bot6={x=1760,y=576},
			  bot7={x=1760,y=876},bot8={x=1760,y=1176},bot9={x=2060,y=576},
			  bot10={x=2060,y=876},bot11={x=2060,y=1176},bot12={x=2240,y=2240}},			  
 
 TIGER_BOT = {bot1={x=2800, y=2800}},
 --RAT_BOT = {bot1={x=2600, y=2600}},
 --ANDROMALIUS_BOSS = {bot1={x=2900, y = 2800}},
 LAVA_BOSS = {bot13={x=1632, y = 2570}},
 MAGE_BOSS = {bot1={x=2240,y=640}}
 
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