

class Scene {
	private:
	int width;
	int height;

	public:
	Scene(int width, int height);
	virtual ~Scene();
	virtual bool render(int framenumber);
};
