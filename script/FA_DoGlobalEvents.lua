-- DoGlobalEvents
-- befor turn
local H3 = require "Homm3.API"
local DBG = require "Homm3.DEBUG"


function FA_DoGlobalEvents( )
	-- body
	idx = H3.GetPlayerIndex()
	str = 'Player ' .. idx .. ' turn'
	--rtv = H3.MessageBox(str, 2, -1, 0, 0)
	DBG.Log(str)
end
