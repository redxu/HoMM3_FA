-- LoadBar
package.cpath = "HoMM3_FA.dll"
local H3 = require "Homm3.API"


function FA_LoadBar( )
	-- body
	local pcx = {
		"loadbar.pcx",
		"zpic106.pcx",
		"zpic1000.pcx",
		"zpic1001.pcx",
		"zpic1005.pcx"
	}

	p = H3.BlitPcx16(pcx[H3.Random(1,5)])
	return p
end
