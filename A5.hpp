// Term--Fall 2020
// Chenyang Zhang

#pragma once

#include <glm/glm.hpp>

#include "cs488-framework/CS488Window.hpp"
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"
#include "cs488-framework/MeshConsolidator.hpp"

#include "SceneNode.hpp"

#include <memory>
#include <vector>
#include "world.hpp"
#include "tree.hpp"

struct LightSource {
	glm::vec3 position;
	glm::vec3 rgbIntensity;
};

class A5 : public CS488Window {
public:
	A5();
	virtual ~A5();

protected:
	virtual void init() override;
	virtual void appLogic() override;
	virtual void guiLogic() override;
	virtual void draw() override;
	virtual void cleanup() override;

	virtual bool cursorEnterWindowEvent(int entered) override;
	virtual bool mouseMoveEvent(double xPos, double yPos) override;
	virtual bool mouseButtonInputEvent(int button, int actions, int mods) override;
	virtual bool mouseScrollEvent(double xOffSet, double yOffSet) override;
	virtual bool windowResizeEvent(int width, int height) override;
	virtual bool keyInputEvent(int key, int action, int mods) override;

	void processLuaFiles();
	void createShaderProgram();
	void enableVertexShaderInputSlots();
	void uploadVertexDataToVbos(const MeshConsolidator & meshConsolidator);
	void mapVboDataToVertexShaderInputLocations();

	void initViewMatrix();
	void initLightSources();
	void initPerspectiveMatrix();

	void uploadCommonSceneUniforms();
	void renderObject(const SceneNode &node, glm::mat4 trans, bool tex_map = false, 
		glm::vec3 n_color = glm::vec3(-1.0f)); // draw from a SceneNode
	void drawScene(bool ambient_only=false); // draw the main objects in this game
	void renderShadowVolume(); // render shadow volume
	void drawOther(); // render other objects
	void drawSkybox(); // render the skybox

	void initTrees(); // generate the 2 trees

private:
	void reset(); // reset the view and the game
	void initMap(); // initialize the game map

	// Fields related to the shader and uniforms.
	ShaderProgram m_shader;
	ShaderProgram m_shadowShader;
	glm::mat4 m_perpsective;
	glm::mat4 m_view;

	LightSource m_light;

	//-- GL resources for mesh geometry data:
	GLuint m_vao_meshData;
	GLuint m_vbo_vertexPositions;
	GLuint m_vbo_vertexNormals;
	GLuint m_vbo_vertexUV;

	GLuint m_fbo_colorDepth;
	GLuint m_depth;
	GLuint m_color;

	GLint m_positionAttribLocation;
	GLint m_normalAttribLocation;
	GLint m_texCoordAttribLocation;

	BatchInfoMap m_batchInfoMap;

	// scene node used
	std::shared_ptr<SceneNode> scene_rootNode;
	std::shared_ptr<SceneNode> block_rootNode;
	std::shared_ptr<SceneNode> tank_rootNode;
	std::shared_ptr<SceneNode> shell_rootNode;
	std::shared_ptr<SceneNode> base_rootNode;

	std::vector<glm::vec2> tex_coord;

	// texture used
	unsigned int texture_stone;
	unsigned int texture_floor;
	unsigned int texture_wall;
	unsigned int texture_up;
	unsigned int texture_down;
	unsigned int texture_left;
	unsigned int texture_right;
	unsigned int texture_front;
	unsigned int texture_back;

	std::vector<LSystem> trees;

	bool texture_mapping = false;
	bool draw_shadow_volume = false;

	std::shared_ptr<World> world;

};
