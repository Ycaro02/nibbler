#ifndef GRAPHICLIB_HPP
#define GRAPHICLIB_HPP

#define SFML_IDX 0
#define SDL2_IDX 1

/* Abstract class for the graphics library */
class GraphicLib { 
	public:
	/* Destructor */
	virtual ~GraphicLib() {}

	/* Initialize the graphics library */
	virtual bool windowCreate() = 0;

	/* Clear the screen */
	virtual void clear() = 0;

	/* Display the rendered content */
	virtual void display() = 0;

	/* Check if the window is open */
	virtual bool isOpen() = 0;

	/* Process events */
	virtual void processEvents(int *currentLib, int *isRunning) = 0;

	/* Close the graphics library */
	virtual void close() = 0;
};

#endif /* GRAPHICLIB_HPP */