struct MaterialData
{
	// Basic colouring and rendering variables
	vec3 ambientColour;
	vec3 diffuseColour;
	vec3 specularColour;
	vec3 emmissionColour;
	float opticalDensity;
	float specularColourWeight;
	float dissolveTransparency;	
	GLuint illuminationModel;
	
	// Texture rendering variables
	char* ambientTextureMap;
	char* diffuseTextureMap;
	char* specularColourTextureMap;
	char* specularHighlightMap;
	char* alphaTextureMap;	
}

struct VertexGroup
{
	vector<vec4> vertexCoordinates;
	vector<vec2> textureCoordinates;
	vector<vec4> normalCoordinates;
	
	MaterialData material;
}

struct ModelObject
{
	vector<VertexGroup> vertexGroups;
}

struct Model
{
	vector<ModelObject> objects;
}

void Parser(vector<string> rawData, Model model)
{
	
}