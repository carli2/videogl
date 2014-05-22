

class Scene {
	private:
	int width;
	int height;

	public:
	Scene(int width, int height) { this->width = width; this->height = height; }
	virtual ~Scene() {};
	virtual bool render(int framenumber) { return false; };
};
