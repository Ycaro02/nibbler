#ifndef GRAPHICLIB_HPP
#define GRAPHICLIB_HPP

/* Abstract class for the graphics library */
class GraphicLib { 
	public:
	/* Destructor */
	virtual ~GraphicLib() {}

	/* Initialize the graphics library */
	virtual bool init() = 0;

	/* Clear the screen */
	virtual void clear() = 0;

	/* Display the rendered content */
	virtual void display() = 0;

	/* Check if the window is open */
	virtual bool isOpen() = 0;

	/* Process events */
	virtual void processEvents() = 0;

	/* Close the graphics library */
	virtual void close() = 0;
};

#endif /* GRAPHICLIB_HPP */