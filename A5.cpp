// Term--Fall 2020
// Chenyang Zhang

#include "A5.hpp"
#include "cs488-framework/GlErrorCheck.hpp"
#include "cs488-framework/MathUtils.hpp"

#include <stb_image.h>

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>
#include <utility>
#include <chrono>

#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "world.hpp"
#include "tank.hpp"
#include "common.hpp"
#include "tree.hpp"

#include "GeometryNode.hpp"
#include "JointNode.hpp"

#include "common/scene_lua.hpp"

using namespace glm;
using namespace std;

static int dim = 11;

const int shapePerStack = 12;
const int stackCount = 6;

bool started = false;

static int height = 1; // height of the wall
int x_pos = 0; // avatar x position
int y_pos = 0; // avatar y position

double prevMouseX = 0; // previous mouse x position
double prevMouseY = 0; // previous mouse y position
double trackXpos = 0;
bool mouseMove = false;
bool mousePress = false; // whether the left button of the mouse is pressed
float rot_value = 0.0f; // rotation value of the maze
double deltaXPos = 0.0; // change of mouse x position
bool persistence = false; // persistence mode
double delta = 0.0;
double scaleFactor = 2.0;

bool show_shadow_volume = false;
bool show_texture_mapping = true;
bool show_shadow = true;
bool draw_enemy = false;
bool draw_skybox = false;
Direction direction = Direction::N;
int op = 100;
int volume = 50;
bool transparent = 1;


//----------------------------------------------------------------------------------------
// Constructor
A5::A5()
	: m_positionAttribLocation(0),
	  m_normalAttribLocation(1),
	  m_texCoordAttribLocation(2),
	  m_vao_meshData(0),
	  m_vbo_vertexPositions(0),
	  m_vbo_vertexNormals(0),
	  m_vbo_vertexUV(0),
	  m_fbo_colorDepth(0),
	  m_depth(0),
	  m_color(0),
	  texture_stone(0),
	  texture_floor(0),
	  texture_wall(0),
	  texture_up(0),
	  texture_down(0),
	  texture_left(0),
	  texture_right(0),
	  texture_front(0),
	  texture_back(0),
	  world(nullptr)
{
	srand(time(NULL));
}

//----------------------------------------------------------------------------------------
// Destructor
A5::~A5()
{

}

//----------------------------------------------------------------------------------------
/*
 * Called once, at program start.
 */
void A5::init()
{
	
	// Set the background colour.
	glClearColor( 0.3, 0.5, 0.7, 1.0 );

	createShaderProgram();

	glGenVertexArrays(1, &m_vao_meshData);
	enableVertexShaderInputSlots();

	processLuaFiles();

	unique_ptr<MeshConsolidator> meshConsolidator (new MeshConsolidator{
			getAssetFilePath("cube.obj"),
			getAssetFilePath("sphere.obj")
	});

	getTexCoord(getAssetFilePath("cube.obj").c_str(), tex_coord);

	texture_stone = loadTexture(getAssetFilePath("mosaic1.jpg").c_str());
	texture_wall = loadTexture(getAssetFilePath("jerusslab1.jpg").c_str());
	texture_floor = loadTexture(getAssetFilePath("grassfull.jpg").c_str());

	texture_up = loadTexture(getAssetFilePath("top.bmp").c_str());
	texture_down = loadTexture(getAssetFilePath("bottom.jpg").c_str());
	texture_left = loadTexture(getAssetFilePath("left.jpg").c_str());
	texture_right = loadTexture(getAssetFilePath("right.jpg").c_str());
	texture_front = loadTexture(getAssetFilePath("front.jpg").c_str());
	texture_back = loadTexture(getAssetFilePath("back.jpg").c_str());

	meshConsolidator->getBatchInfoMap(m_batchInfoMap);

	uploadVertexDataToVbos(*meshConsolidator);

	mapVboDataToVertexShaderInputLocations();

	initPerspectiveMatrix();

	initViewMatrix();

	initLightSources();

	initMap();
	initTrees();
}

//----------------------------------------------------------------------------------------
void A5::processLuaFiles() {
	scene_rootNode = shared_ptr<SceneNode>(import_lua("Assets/scene.lua"));
	if (!scene_rootNode) {
		cerr << "Could Not Open scene lua file" << endl;
	}

	block_rootNode = shared_ptr<SceneNode>(import_lua("Assets/block.lua"));
	if (!block_rootNode) {
		cerr << "Could Not Open block lua file" << endl;
	}

	tank_rootNode = shared_ptr<SceneNode>(import_lua("Assets/tank.lua"));
	if (!tank_rootNode) {
		cerr << "Could Not Open tank lua file" << endl;
	}

	shell_rootNode = shared_ptr<SceneNode>(import_lua("Assets/shell.lua"));
	if (!shell_rootNode) {
		cerr << "Could Not Open shell lua file" << endl;
	}

	base_rootNode = shared_ptr<SceneNode>(import_lua("Assets/puppet.lua"));
	if (!base_rootNode) {
		cerr << "Could Not Open shell lua file" << endl;
	}
}

//----------------------------------------------------------------------------------------
void A5::createShaderProgram()
{
	m_shader.generateProgramObject();
	m_shader.attachVertexShader( getAssetFilePath("VertexShaderP.vs").c_str() );
	m_shader.attachFragmentShader( getAssetFilePath("FragmentShaderP.fs").c_str() );
	m_shader.link();

	m_shadowShader.generateProgramObject();
	m_shadowShader.attachVertexShader( getAssetFilePath("shadowV.vs").c_str() );
	m_shadowShader.attachFragmentShader( getAssetFilePath("shadowF.fs").c_str() );
	m_shadowShader.attachGeometryShader(getAssetFilePath("shadowG.gs").c_str());
	m_shadowShader.link();
}

//----------------------------------------------------------------------------------------
void A5::enableVertexShaderInputSlots()
{
	//-- Enable input slots for m_vao_meshData:
	{
		glBindVertexArray(m_vao_meshData);

		// Enable the vertex shader attribute location for "position" when rendering.
		m_positionAttribLocation = 0;
		glEnableVertexAttribArray(m_positionAttribLocation);

		// Enable the vertex shader attribute location for "normal" when rendering.
		m_normalAttribLocation = 1;
		glEnableVertexAttribArray(m_normalAttribLocation);

		// Enable the vertex shader attribute location for "aTexCoord" when rendering.
		m_texCoordAttribLocation = 2;
		glEnableVertexAttribArray(m_texCoordAttribLocation);

		CHECK_GL_ERRORS;
	}

	// Restore defaults
	glBindVertexArray(0);
}

//----------------------------------------------------------------------------------------
void A5::uploadVertexDataToVbos (
		const MeshConsolidator & meshConsolidator
) {
	// Generate VBO to store all vertex position data
	{
		glGenBuffers(1, &m_vbo_vertexPositions);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexPositions);

		glBufferData(GL_ARRAY_BUFFER, meshConsolidator.getNumVertexPositionBytes(),
				meshConsolidator.getVertexPositionDataPtr(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		CHECK_GL_ERRORS;
	}

	// Generate VBO to store all vertex normal data
	{
		glGenBuffers(1, &m_vbo_vertexNormals);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexNormals);

		glBufferData(GL_ARRAY_BUFFER, meshConsolidator.getNumVertexNormalBytes(),
				meshConsolidator.getVertexNormalDataPtr(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		CHECK_GL_ERRORS;
	}

	// Generate VBO to store all vertex texture mapping data
	{
		glGenBuffers(1, &m_vbo_vertexUV);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexUV);

		glBufferData(GL_ARRAY_BUFFER, tex_coord.size() * sizeof(vec2),
				&(tex_coord[0].x), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		CHECK_GL_ERRORS;
	}
}

//----------------------------------------------------------------------------------------
void A5::mapVboDataToVertexShaderInputLocations()
{
	// Bind VAO in order to record the data mapping.
	glBindVertexArray(m_vao_meshData);

	// Tell GL how to map data from the vertex buffer "m_vbo_vertexPositions" into the
	// "position" vertex attribute location for any bound vertex shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexPositions);
	glVertexAttribPointer(m_positionAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Tell GL how to map data from the vertex buffer "m_vbo_vertexNormals" into the
	// "normal" vertex attribute location for any bound vertex shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexNormals);
	glVertexAttribPointer(m_normalAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Tell GL how to map data from the vertex buffer "m_vbo_vertexUV" into the
	// "aTexCoord" vertex attribute location for any bound vertex shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexUV);
	glVertexAttribPointer(m_texCoordAttribLocation, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	//-- Unbind target, and restore default values:
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}


//----------------------------------------------------------------------------------------
void A5::uploadCommonSceneUniforms() {
	m_shader.enable();

	//-- Set Perpsective matrix uniform for the scene:
	GLint location = m_shader.getUniformLocation("Perspective");
	glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(m_perpsective));
	CHECK_GL_ERRORS;

	{
		//-- Set LightSource uniform for the scene:
		location = m_shader.getUniformLocation("light.position");
		glUniform3fv(location, 1, value_ptr(m_light.position));
		location = m_shader.getUniformLocation("light.rgbIntensity");
		glUniform3fv(location, 1, value_ptr(m_light.rgbIntensity));
		CHECK_GL_ERRORS;

		location = m_shader.getUniformLocation("ambientIntensity");
		vec3 ambientIntensity(0.15f);
		glUniform3fv(location, 1, value_ptr(ambientIntensity));
		CHECK_GL_ERRORS;
	}

	m_shader.disable();

	m_shadowShader.enable();

	location = m_shadowShader.getUniformLocation("Perspective");
	glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(m_perpsective));
	CHECK_GL_ERRORS;
	location = m_shadowShader.getUniformLocation("lightPos");
	vec3 true_light_pos = vec3(m_view * vec4(m_light.position, 1.0f));
	glUniform3fv(location, 1, value_ptr(true_light_pos));
	CHECK_GL_ERRORS;

	m_shadowShader.disable();

}

//----------------------------------------------------------------------------------------
void A5::initPerspectiveMatrix()
{
	float aspect = ((float)m_windowWidth) / m_windowHeight;
	m_perpsective = glm::perspective(degreesToRadians(40.0f), aspect, 0.1f, 100.0f);
}


//----------------------------------------------------------------------------------------
void A5::initViewMatrix() {
	m_view = glm::lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f),
			vec3(0.0f, 1.0f, 0.0f));
}

//----------------------------------------------------------------------------------------
void A5::initLightSources() {
	// World-space position
	m_light.position = vec3(15.0f, 10.0f, 10.0f);
	m_light.rgbIntensity = vec3(0.9f); // light
}

void A5::initTrees() {
	LSystem new_tree;
	new_tree.initGrammar();
    new_tree.generateTree(translate(mat4(1.0f), vec3(10,-1,0)));
	trees.push_back(new_tree);
	new_tree.clearAll();
	new_tree.initGrammar();
	new_tree.generateTree(translate(mat4(1.0f), vec3(-10,-1,0)));
	trees.push_back(new_tree);
}

//----------------------------------------------------------------------------------------
void A5::renderObject(const SceneNode & node, glm::mat4 trans, bool tex_map, vec3 n_color) {

	trans = trans * node.get_transform();
	texture_mapping = tex_map;
	if (node.m_nodeType == NodeType::GeometryNode) {
		const GeometryNode * geometryNode = static_cast<const GeometryNode *>(&node);
		if (draw_shadow_volume) {
			updateShaderUniforms(m_shadowShader, *geometryNode, m_view, 
				trans, texture_mapping, draw_shadow_volume, show_texture_mapping, n_color,
				float(op)/100.0f+transparent, draw_skybox);
		}
		else {
			updateShaderUniforms(m_shader, *geometryNode, m_view, 
				trans, texture_mapping, draw_shadow_volume, show_texture_mapping,
				n_color, float(op)/100.0f+transparent, draw_skybox);
		}


		// Get the BatchInfo corresponding to the GeometryNode's unique MeshId.
		BatchInfo batchInfo = m_batchInfoMap[geometryNode->meshId];
		//-- Now render the mesh:
		if (draw_shadow_volume) {
			m_shadowShader.enable();
			glDrawArrays(GL_TRIANGLES, batchInfo.startIndex, batchInfo.numIndices);
			m_shadowShader.disable();
		}
		else {
			m_shader.enable();
			glDrawArrays(GL_TRIANGLES, batchInfo.startIndex, batchInfo.numIndices);
			m_shader.disable();
		}
		

	}

	for (const SceneNode * n: node.children) {
		renderObject(*n, trans, tex_map, n_color);
	}
	
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, before guiLogic().
 */
void A5::appLogic()
{
	// Place per frame, application logic here ...
	mouseMove = !(trackXpos == prevMouseX);
	trackXpos = prevMouseX;
	if (persistence) {
		rot_value += delta * ImGui::GetIO().DeltaTime;
	}
#ifdef ENABLE_SOUND
	world->sound_player->setVolume(float(volume)/100.0f);
#endif
	// world->update();
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after appLogic(), but before the draw() method.
 */
void A5::guiLogic()
{

	static bool showTestWindow(false);
	static bool showDebugWindow(true);

	ImGuiWindowFlags windowFlags(ImGuiWindowFlags_AlwaysAutoResize);
	float opacity(0.5f);


	ImGui::Begin("Debug Window", &showDebugWindow, ImVec2(100,100), opacity, windowFlags);
		if( ImGui::Button( "Quit Application" ) ) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
		}
		if (ImGui::Button("Reset (R)")) {
			reset();
		}
		if (ImGui::Button("Start (S)")) {
			world->start();
		}
		if (ImGui::Button("Retry (N)")) {
			initMap();
		}
		

		if (world->isGameover()) {
			ImGui::Begin("Game", &showDebugWindow, ImVec2(100, 100), opacity, windowFlags);
				ImGui::Text("GAME OVER");
			ImGui::End();
		}
		ImGui::Checkbox("Show Shadow Volume", &show_shadow_volume);
		ImGui::Checkbox("Show Shadow", &show_shadow);
		ImGui::Checkbox("Texture Mapping", &show_texture_mapping);
		ImGui::SliderInt("Volume", &volume, 0, 100, to_string(volume).c_str());
		ImGui::SliderInt("Opacity", &op, 0, 100, (to_string(op)).c_str());

/*
		// For convenience, you can uncomment this to show ImGui's massive
		// demonstration window right in your application.  Very handy for
		// browsing around to get the widget you want.  Then look in 
		// shared/imgui/imgui_demo.cpp to see how it's done.
		if( ImGui::Button( "Test Window" ) ) {
			showTestWindow = !showTestWindow;
		}
*/
		ImGui::Text("Score %d", world->score);
		ImGui::Text("Health %d", world->player->getHP());
		ImGui::Text( "Framerate: %.1f FPS", ImGui::GetIO().Framerate );

	ImGui::End();

	if( showTestWindow ) {
		ImGui::ShowTestWindow( &showTestWindow );
	}
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after guiLogic().
 */
void A5::draw()
{


	mat4 r_trans = mat4(1.0f);
	mat4 c_trans = mat4(1.0f);
	c_trans = glm::translate(c_trans, vec3(0,0,0));
	c_trans = glm::rotate(c_trans, rot_value, vec3(0,1,0));

	
	m_view = glm::lookAt( 
		glm::vec3( 0.0, 1.6*15*scaleFactor*M_SQRT1_2, 
			0.8*scaleFactor*15*M_SQRT1_2),
		glm::vec3( 0.0f, 0.0f, 0.0f ),
		glm::vec3( 0.0f, 1.0f, 0.0f ) );

	uploadCommonSceneUniforms();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glDepthMask(GL_TRUE);
	glDisable(GL_STENCIL_TEST);
	glEnable( GL_DEPTH_TEST );

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	drawSkybox();
	glDisable(GL_CULL_FACE);

	if (show_shadow) {


		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		drawScene(true);

		glDisable(GL_CULL_FACE);

		glEnable(GL_STENCIL_TEST);
		glEnable(GL_DEPTH_TEST);

		glStencilFunc(GL_ALWAYS, 0x0, 0xFF);
		glEnable(GL_DEPTH_CLAMP);

		if (!show_shadow_volume) {
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
			// glDepthMask(GL_FALSE);
		}

		glStencilOpSeparate(GL_BACK, GL_KEEP, GL_KEEP, GL_INCR_WRAP);
		glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_KEEP, GL_DECR_WRAP);

		renderShadowVolume();

		glDisable(GL_DEPTH_CLAMP);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDepthFunc(GL_EQUAL);
		glStencilFunc(GL_EQUAL, 0x0, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);


		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		drawScene();

		glDisable(GL_CULL_FACE);

		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);
		glDisable(GL_STENCIL_TEST);
	}
	else {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		drawScene();
		glDisable(GL_CULL_FACE);
	}

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	drawOther();
	glDisable(GL_CULL_FACE);

	CHECK_GL_ERRORS;

	world->update();
}

void A5::drawScene(bool ambient_only) {

	if (ambient_only) {
		m_shader.enable();

		GLint location = m_shader.getUniformLocation("ambient_only");
		glUniform1i(location, true);
		CHECK_GL_ERRORS;

		m_shader.disable();
	}

	mat4 r_trans = mat4(1.0f);
	mat4 c_trans = mat4(1.0f);
	c_trans = glm::translate(c_trans, vec3(0,0,0));
	c_trans = glm::rotate(c_trans, rot_value, vec3(0,1,0));
	
	
	glBindVertexArray(m_vao_meshData);

	// draw the player's tank
	if (!world->isGameover()) {
		vec3 player_pos = world->getPlayerPos();
		mat4 playerTrans = world->getRootTrans();
		r_trans = glm::translate(c_trans, 
			vec3( player_pos[0]-float(dim)/2.0f-0.5f, 0.75f, player_pos[2]-float(dim)/2.0f-0.5f));
		r_trans = r_trans * playerTrans;
		renderObject(*tank_rootNode, r_trans);
	}

	for (auto e: world->enemy) {
		vec3 e_pos = e->getPos();
		mat4 e_trans = e->getRootTrans();
		r_trans = glm::translate(c_trans, 
		vec3( e_pos[0]-float(dim)/2.0f-0.5f, 0.75f, e_pos[2]-float(dim)/2.0f-0.5f));
		r_trans = r_trans * e_trans;
		renderObject(*tank_rootNode, r_trans, false, vec3(0.1,0.2,0.1));
	}

	// 
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_floor);
	renderObject(*scene_rootNode, c_trans, true);

	// Draw the destroyable blocks
	glBindTexture(GL_TEXTURE_2D, texture_stone);
	for (auto b: world->getBlocks()) {
		vec3 block_pos = b->getPos();
		r_trans = glm::translate(c_trans, 
			vec3( block_pos[0]-float(dim)/2.0f, 1.0f, block_pos[2]-float(dim)/2.0f));
		if (b->isDestroyable()) {
			renderObject(*block_rootNode, r_trans, true);
		}
	}

	// Draw the undestroyable blocks
	glBindTexture(GL_TEXTURE_2D, texture_wall);
	for (auto b: world->getBlocks()) {
		vec3 block_pos = b->getPos();
		r_trans = glm::translate(c_trans, 
			vec3( block_pos[0]-float(dim)/2.0f, 1.0f, block_pos[2]-float(dim)/2.0f));
		if (!b->isDestroyable()) {
			renderObject(*block_rootNode, r_trans, true);
		}
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	// draw the shells

	for (auto s: world->getShells()) {
		vec3 s_pos = s->getPos();
		r_trans = glm::translate(c_trans, 
			vec3( s_pos[0]-float(dim)/2.0f-0.5f, 0.75f, s_pos[2]-float(dim)/2.0f-0.5f));
		if (s->getSide()) {
			renderObject(*shell_rootNode, r_trans, false, vec3(0.8f, 0.8f, 0.1f));
		}
		else {
			renderObject(*shell_rootNode, r_trans);
		}
	}

	if (!world->isGameover()) {
		// draw the base (a puppet)
		pair<int, int> base_pos = world->getBasePos();
		r_trans = glm::translate(c_trans, 
			vec3( base_pos.first-float(dim)/2.0f, 1.f, base_pos.second-float(dim)/2.0f));
		renderObject(*base_rootNode, r_trans);
	}

	for (auto t: trees) {
		// cout << t.getTrunks().size() << endl;
		for (auto trunk: t.getTrunks()) {
			r_trans = trunk.trans;
			r_trans = c_trans * r_trans;
			// cout << to_string(r_trans) << endl;
			renderObject(*block_rootNode, r_trans, false, vec3(0.4,0.25,0.125));

		}
	}
	
	

	for (auto ps: world->explosions) {
		for (auto p_trans: ps->getTrans()) {
			r_trans = p_trans;
			// cout << to_string(p_trans) << endl;
			r_trans = c_trans * r_trans;
			renderObject(*block_rootNode, r_trans, false, vec3(0.5f,0.1f,0.1f));
		}
	}

	// Restore defaults
	glBindVertexArray( 0 );

	CHECK_GL_ERRORS;

	m_shader.enable();

	GLint location = m_shader.getUniformLocation("ambient_only");
	glUniform1i(location, false);
	CHECK_GL_ERRORS;

	m_shader.disable();
}

void A5::renderShadowVolume() {
	mat4 r_trans = mat4(1.0f);
	mat4 c_trans = mat4(1.0f);
	c_trans = glm::translate(c_trans, vec3(0,0,0));
	c_trans = glm::rotate(c_trans, rot_value, vec3(0,1,0));
	draw_shadow_volume = true;

	glBindVertexArray(m_vao_meshData);

	for (auto b: world->getBlocks()) {
		vec3 block_pos = b->getPos();
		r_trans = glm::translate(c_trans, 
			vec3( block_pos[0]-float(dim)/2.0f, 1.0f, block_pos[2]-float(dim)/2.0f));
		if (b->isDestroyable()) {
			renderObject(*block_rootNode, r_trans, true);
		}
	}

	// Draw the undestroyable blocks
	glBindTexture(GL_TEXTURE_2D, texture_wall);
	for (auto b: world->getBlocks()) {
		vec3 block_pos = b->getPos();
		r_trans = glm::translate(c_trans, 
			vec3( block_pos[0]-float(dim)/2.0f, 1.0f, block_pos[2]-float(dim)/2.0f));
		if (!b->isDestroyable()) {
			renderObject(*block_rootNode, r_trans, true);
		}
	}

	draw_shadow_volume = false;
	glBindVertexArray(0);


}

void A5::drawOther() {
	glBindVertexArray(m_vao_meshData);

	mat4 r_trans = mat4(1.0f);
	mat4 c_trans = mat4(1.0f);
	c_trans = glm::translate(c_trans, vec3(0,0,0));
	c_trans = glm::rotate(c_trans, rot_value, vec3(0,1,0));

	transparent = 0;

	r_trans = mat4(1.0f);
	r_trans = glm::translate(r_trans, 
			vec3( -10.0f, 6.0f, 0.0f));
	r_trans = scale(r_trans, vec3(25,30,25));
	r_trans = c_trans * r_trans;
	renderObject(*shell_rootNode, r_trans, false, vec3(0.3, 0.7, 0.3));

	r_trans = mat4(1.0f);
	r_trans = glm::translate(r_trans, 
			vec3( 10.0f, 6.0f, 0.0f));
	r_trans = scale(r_trans, vec3(25,30,25));
	r_trans = c_trans * r_trans;
	renderObject(*shell_rootNode, r_trans, false, vec3(0.3, 0.7, 0.3));

	transparent = 1;

	glBindVertexArray(0);
}

void A5::drawSkybox() {
	glBindVertexArray(m_vao_meshData);

	draw_skybox = true;

	mat4 r_trans = mat4(1.0f);
	mat4 c_trans = mat4(1.0f);
	c_trans = glm::translate(c_trans, vec3(0,0,0));
	c_trans = glm::rotate(c_trans, rot_value, vec3(0,1,0));

	glBindTexture(GL_TEXTURE_2D, texture_up);
	r_trans = mat4(1.0f);
	
	r_trans = glm::translate(r_trans, 
			vec3( 0.0f, 50.0f, 0.0f));
	r_trans = scale(r_trans, vec3(50,50,50));
	r_trans = c_trans * r_trans;
	renderObject(*block_rootNode, r_trans, true);

	glBindTexture(GL_TEXTURE_2D, texture_down);
	r_trans = mat4(1.0f);
	r_trans = rotate(r_trans, degreesToRadians(180.0f), vec3(0,1,0));
	r_trans = glm::translate(r_trans, 
			vec3( 0.0f, -50.0f, 0.0f));
	r_trans = scale(r_trans, vec3(50,50,50));
	r_trans = c_trans * r_trans;
	renderObject(*block_rootNode, r_trans, true);

	glBindTexture(GL_TEXTURE_2D, texture_left);
	r_trans = mat4(1.0f);
	r_trans = glm::translate(r_trans, 
			vec3( 50.0f, 0.0f, 0.0f));
	r_trans = scale(r_trans, vec3(50,50,50));
	r_trans = c_trans * r_trans;
	renderObject(*block_rootNode, r_trans, true);

	glBindTexture(GL_TEXTURE_2D, texture_right);
	r_trans = mat4(1.0f);
	r_trans = glm::translate(r_trans, 
			vec3( -50.0f, 0.0f, 0.0f));
	r_trans = scale(r_trans, vec3(50,50,50));
	r_trans = c_trans * r_trans;
	renderObject(*block_rootNode, r_trans, true);

	glBindTexture(GL_TEXTURE_2D, texture_front);
	r_trans = mat4(1.0f);
	r_trans = glm::translate(r_trans, 
			vec3( 0.0f, 0.0f, 50.0f));
	r_trans = scale(r_trans, vec3(50,50,50));
	r_trans = c_trans * r_trans;
	renderObject(*block_rootNode, r_trans, true);

	glBindTexture(GL_TEXTURE_2D, texture_back);
	r_trans = mat4(1.0f);
	r_trans = glm::translate(r_trans, 
			vec3( 0.0f, 0.0f, -50.0f));
	r_trans = scale(r_trans, vec3(50,50,50));
	r_trans = c_trans * r_trans;
	renderObject(*block_rootNode, r_trans, true);

	draw_skybox = false;

	glBindVertexArray(0);
}

//----------------------------------------------------------------------------------------
/*
 * Called once, after program is signaled to terminate.
 */
void A5::cleanup()
{

}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles cursor entering the window area events.
 */
bool A5::cursorEnterWindowEvent (
		int entered
) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse cursor movement events.
 */
bool A5::mouseMoveEvent(double xPos, double yPos) 
{
	bool eventHandled(false);

	if (!ImGui::IsMouseHoveringAnyWindow()) {

		// Put some code here to handle rotations.  Probably need to
		// check whether we're *dragging*, not just moving the mouse.
		// Probably need some instance variables to track the current
		// rotation amount, and maybe the previous X position (so 
		// that you can rotate relative to the *change* in X.
		deltaXPos = xPos - prevMouseX;
		if (mousePress) {
			rot_value = rot_value + deltaXPos * 0.005;
			rot_value = fmod(rot_value, 360.0);
		}
		prevMouseX = xPos;
		prevMouseY = yPos;
	}

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse button events.
 */
bool A5::mouseButtonInputEvent(int button, int actions, int mods) {
	bool eventHandled(false);

	if (!ImGui::IsMouseHoveringAnyWindow()) {
		// The user clicked in the window.  If it's the left
		// mouse button, initiate a rotation.
		if ((button == GLFW_MOUSE_BUTTON_LEFT) && (actions == GLFW_PRESS)) {
			mousePress = true;
			persistence = false;
		}
		else if ((button == GLFW_MOUSE_BUTTON_LEFT) && (actions == GLFW_RELEASE)) {
			mousePress = false;
			if (mouseMove) {
				persistence = true;
				delta = deltaXPos * 0.005 / ImGui::GetIO().DeltaTime;
			}
		}
	}

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse scroll wheel events.
 */
bool A5::mouseScrollEvent(double xOffSet, double yOffSet) {
	bool eventHandled(false);

	// Zoom in or out.
	if (yOffSet == 1) {
		if (scaleFactor <= 1) {}
		else if (scaleFactor > 2) {
			scaleFactor -= 0.1;
		}
		else {
			scaleFactor -= 0.05;
		}
	}
	else if (yOffSet == -1) {
		if (scaleFactor >= 2.5) {}
		else if (scaleFactor < 2) {
			scaleFactor += 0.05;
		}
		else {
			scaleFactor += 0.1;
		}
	}
	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles window resize events.
 */
bool A5::windowResizeEvent(int width, int height) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles key input events.
 */
bool A5::keyInputEvent(int key, int action, int mods) {
	bool eventHandled(false);

	// Fill in with event handling code...
	if( action == GLFW_PRESS ) {
		// Respond to some key events.
		vec3 player_pos = world->getPlayerPos();
		x_pos = player_pos[0];
		y_pos = player_pos[2];
		switch (key) {
			case GLFW_KEY_N: {
				initMap();
				started = false;
				break;
			}
			case GLFW_KEY_Q: {
				glfwSetWindowShouldClose(m_window, GL_TRUE);
				break;
			}
			case GLFW_KEY_R: {
				reset();
				break;
			}
			case GLFW_KEY_S: {
				world->start();
				break;
			}

			case GLFW_KEY_SPACE: {
				world->event(4);
				break;
			}

			case GLFW_KEY_UP: {
				world->event(0);

				break;
			}
			case GLFW_KEY_DOWN: {
				world->event(1);

				break;
			}
			case GLFW_KEY_LEFT: {
				world->event(2);

				break;
			}
			case GLFW_KEY_RIGHT: {
				world->event(3);

				break;
			}
		}
		
	}
	else if (action == GLFW_RELEASE) {}
	else if (action == GLFW_REPEAT) {
		switch (key) {
			case GLFW_KEY_SPACE: {
				world->event(4);
				break;
			}
			case GLFW_KEY_UP: {
				world->event(0);

				break;
			}
			case GLFW_KEY_DOWN: {
				world->event(1);

				break;
			}
			case GLFW_KEY_LEFT: {
				world->event(2);

				break;
			}
			case GLFW_KEY_RIGHT: {
				world->event(3);

				break;
			}
		}
	}

	return eventHandled;
}

void A5::reset() {

	started = 0;

	if (world) {
		world->reset();
		initMap();
	}

	mouseMove = false;
	mousePress = false;
	rot_value = 0.0f;
	deltaXPos = 0.0;
	persistence = false;
	delta = 0.0;
	scaleFactor = 2.0;


	height = 1;

	x_pos = 0;
	y_pos = 0;

}

void A5::initMap() {

	if (world) {
		world->reset();
	}
	else {
		world = make_shared<World>();
	}
	world->readMap();
	// world->printMap();

	dim = world->getDim();
}

