

class Scene {
	protected:
	int width;
	int height;
	int fps;

	public:
	Scene(int width, int height, int fps) {
		this->width = width;
		this->height = height;
		this->fps = fps;
	}
	virtual ~Scene() { };
	virtual bool render(int framenumber) {
		return false;
	};
};
