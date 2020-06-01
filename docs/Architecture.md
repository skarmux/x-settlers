static class MapLoader

For MapOverview
-> List of MapInfo Structs each containing:
	-> name
	-> dimensions
	-> player limit
	-> resource preset
	
	-> campaign flag
	-> quest text
	-> quest tip

For PlayField
-> MapVertexBuffer
-> Settler Positions
-> Building Positions
-> Resource Positions

X-Settlers App wants to load Map Overview
X-Settlers selects one of the MapInfo Structs and wants to show further Information
X-Settlers uses the MapInfo to create a PlayField and run the Game