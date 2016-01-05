-- DoGlobalEvents
-- befor turn
local H3 = require "Homm3.API"
local LOG = require "Homm3.LOG"


function FA_DoGlobalEvents( )
	-- body
	idx = H3.GetPlayerIndex()
	str = 'Player ' .. idx .. ' trun'
	LOG.Log(str);
	--H3.MessageBox(str, 1, 0)
end