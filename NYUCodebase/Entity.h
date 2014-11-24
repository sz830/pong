class Entity {
public:
	void Draw();
	float x=0;
	float y=0;
	float rotation=0;

	int textureID;

	float width;
	float height;
	float velocity_x=0;
	float velocity_y=0;
};

void Entity::Draw() {
	//init
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	
	GLfloat vertices[] = {	-width / 2.f,  height / 2.f, 
							-width / 2.f, -height / 2.f, 
							 width / 2.f, -height / 2.f, 
							 width / 2.f,  height / 2.f };

	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glEnableClientState(GL_VERTEX_ARRAY);
	

	GLfloat quadUVs[] = { 0, 0, 0, 1, 1, 1, 1, 0 };
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glRotatef(rotation, 0.0, 0.0, 1.0);
	glDrawArrays(GL_QUADS, 0, 4);

	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_BLEND);
}