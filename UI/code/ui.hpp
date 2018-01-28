/*!
 \file ui.hpp
 \brief Contains prototypes of UI functions
*/

#ifndef UI_HPP
#define UI_HPP

#include <conio.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <iostream.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <new.h>
#include <process.h>

//! Validator function that's used for validating user input
typedef int (*validator_f)(const char *);

//! For running ui::init() before main (initialising basic stuff)
class init_lib_ui
{
	static int counter; //!< Ensures ui::init() is called only once
	public:
		init_lib_ui(); //!< Ctor
};

//! Static object of type init_lib_ui that is initialised
//! before main is run and thus, ui::init is called
static init_lib_ui init_obj_ui;

//! Manipulator class to manipulate UI functions
/*!
 Objects of this type would be used instead of an enum
 to avoid conflicts with int
 Every manipulator object is identified by its index while
 static index indicates the index to be assigned to the next
 manipulator
*/
class manipulator
{
	static int index; //!< index of a new manipulator object
	int own_index;	  //!< index of current manipulator

	public:
		manipulator(); //!< Ctor; assigns index
		int operator==(manipulator); //!< Returns 1 if indexes are same
};

//! Class containing basic UI functions and attributes
class ui
{
	ui();	//!< Private ctor; object of this class shouldn't be created
	public:
	
		//! Specifies the directions for modifying frame, etc.
		enum dir
		{
			left = 1,
			top = 2,
			right = 4,
			bottom = 8,
			all = 16 //!< When all sides need to be modified
		};
		static int scr_height; //!< Height of screen
		static int scr_width; //!< Width of screen
		static void init(); //!< Sets all static variables
		static void clrscr(); //!< Clears the contents off the screen
		static int tcolor; //!< text color
		static int bcolor; //!< background color
		static manipulator endl; //!< End line and move cursor to next line
		static manipulator centeralign; //!< Center align
		static manipulator rightalign; //!< Right align

		//! This func is called when new is unable to allocate memory
		static void my_new_handler();
};

//! Represents a coordinate
struct coord
{
	int x;	//!< x coordinate
	int y;	//!< y coordinate

	coord(int = 1,int = 1); //!< Sets the coordinate
	coord & operator+=(coord);
	coord & operator-=(coord);
	coord operator+(coord); 
	coord operator-(coord);
};

//! Represents the node of a list representing the layout
/*!
 Represents all the information of an element that will be
 printed on the screen. Also points to the next element of the
 screen that will be printed next to it
*/
class list_layout_node
{
    list_layout_node *next;     //!< Pointer to next node
    coord pos;                  //!< Position where to print
    int tcolor;                 //!< Text colour
    int bcolor;                 //!< Background colour
	char str[100];              //!< String to print

	//! How to print the string; mainly for passwords
	int print_type;				

    public:
        list_layout_node();     //!< Ctor
        ~list_layout_node();    //!< Dtor

        //!@{ Setter functions
        void setnext(list_layout_node *);
        void setpos(coord);
        void settcolor(int);
        void setbcolor(int);
		void setstr(const char *);
		void setprint_type(int);
		//!@}

        //!@{ Getter functions
        list_layout_node * getnext();
        coord getpos();
        int gettcolor();
        int getbcolor();
		const char * getstr();
		int getprint_type();
		//!@}
		
		//! Used to distinguish will be printed i.e.
		//! as is or hidden (as passwords)
		enum print_types
		{
			DEFAULT,
			PASSWORD
		};
};

//! A node of the representation of string as a linked list
struct string_node
{
    string_node *next;	//!< Pointer to next node
    string_node *prev;	//!< Pointer to previous node
    char data;			//!< Character stored in string

    string_node();		//!< Ctor
};

//! Represents all interactive information
/*!
 Basically a parent class of all the classes that
 represent the elements of the layout the user can
 interact with.
 Used so that all those elements can be clubbed together
 and the input be taken.
*/
class interactive : public list_layout_node
{
	interactive *prev;    	//!< ptr to previous node
	interactive *next;    	//!< ptr to next node
	int offset;		   	  	//!< offset to y position when printing
	public:
		interactive();		//!< Ctor
		~interactive();		//!< Dtor

		//! Empty input function that will be overridden by children
		/*!
		 \param offset The offset to y position
		 \return Action that was performed by the user
		*/
		virtual int input(int offset);

		//! Setter function
		void setoffset(int);

		//! Getter function
		int getoffset();

		//! Actions that are performed by user; returned from input func.
		enum actions
        {
            GOTONEXT,
			GOTOPREV,
			CLICKED,
			BACK //!< When shift-bckspc is pressed
		};
		
		//! Keys that user can press to navigate the form
		enum keys
		{
			TAB,
			ENTER,
			BACKSPACE,
			SHIFT_BACKSPACE,
			SHIFT_TAB,
			HOME,
			END,
			DELETE,
			UP,
			DOWN,
			LEFT,
			RIGHT
		};

		//! Gets key from user and returns code
		/*
		 \return Keyname corresponding to enum keys
		*/
		static int getkey();
};

//! Represents a text box
/*!
 Inherits from interactive as a text box can be interacted
 with. Gets data from user and stores it as a string that
 can be further converted to the required data type
*/
class text_box : public interactive
{
	//! Represents if the data entered in the text box
	//! should be displayed as is or replaced with asterisks
	int is_password;

	public:
		text_box(); //!< Ctor

		//! Takes input and returns user action
		/*!
		 /param offset Offset of y coordinate to print
		 /return Action performed by user
		*/
        int input(int offset = 0);

		//! Prints string represented by a linked list
		/*
		 Takes in the head pointer of the linked list
		 string and prints the string by iterating through
		 the list. Has no other side effects.
		 /param head ptr to head of the linked list
		*/
		void print_str(string_node *head);

		//! Setter function
		void setis_password(int);
};

//! Represents a button that can be clicked
/*!
 Inherits from interactive as a button can be interacted with.
 A user can click the button while it's input function is
 running which will return the user action
*/
class button : public interactive
{
	int tcolor_selected; //!< tcolor when selected
	int bcolor_selected; //!< bcolor when seilected

	public:
		button(); //!< Ctor

		//!@{ Setter functions
		void settcolor_selected(int);
		void setbcolor_selected(int);
		//!@}

		//!@{ Getter functions
		int gettcolor_selected();
		int getbcolor_selected();
		//!@}

		//! Input function
		/*!
		 Effectively allows the button to be clicked
		 /param offset Offset of y coordinate to print
		 /return Action performed by the user
		*/
		int input(int offset = 0);

		//! Prints the button
		/*!
		 /param isselected Indicates if button is selected or not
		*/						
		void print(int isselected = 0);
};

//! Represents the layout of the page
/*!
 Incorporates elements like simple nodes as well as other
 interactive elements. This layout can be contained within
 a specific height and the overflowing content can reached
 by scrolling which is also implemented here.
*/
class list_layout
{
	//!@{ Pointers to implement a linked list to elements
    list_layout_node *head;	//!< ptr to head node
	list_layout_node *current; //!< ptr to current node
	//!@}

	coord corner_top_left; //!< top left corner of container
	
	/*!
     Following are used as temporary placeholders till data
	 is written to the nodes
	*/
	///!@{
	coord pos;
	int tcolor;
	int bcolor;
	int tcolor_selected;
	int bcolor_selected;
	int tcolor_input;
	int bcolor_input;
	///!@}

	//!@{ For scrolling implementation
	int height; //!< Height of the layout
	int width; //!< Width of the layout
	int lines_scrolled; //!< Lines currently scrolled
	//!@}

	//! For better verbosity at internal level
	enum print_modes
	{
		DISPLAY, 
		HIDE
	};

	//! Prints the layout
	/*!
	 Prints the layout by iterating through the internal
	 linked list maintained. Has no other side effects
	 /param print_mode How to print the data
	*/
	void print(int print_mode = DISPLAY);
    public:
		list_layout(); //!< Ctor
		
		//!@{ Set an element (node)
		list_layout& operator<<(coord); //!< Set coord of node

		//! Set data held by the node
		list_layout& operator<<(const char *);
		//!@}

		//! Set a text box
		/*!
		 Sets a text box at the position indicated by pos and
		 returns a pointer to it
		 /param pos Position at which to set text box
		 /param is_pass If the text box has a password, set to 1
		 /return pointer to the text box set (casted to interactive *)
		*/
		interactive * settext_box(coord pos, int is_pass = 0);

		//! Set a button
		/*!
		 Sets a button at the position indicated by pos and 
		 returns a pointer to it
		 /param pos Position at which to set the button
		 /param txt The text the button displays
		*/
		interactive * setbutton(coord pos, const char *txt);

		//!@{ Setter functions
		void settcolor(int);
		void setbcolor(int);
		void settcolor_selected(int);
		void setbcolor_selected(int);
		void settcolor_input(int);
		void setbcolor_input(int);
		void setcorner_top_left(coord);
		void setheight(int);
		void setwidth(int);
		void setlines_scrolled(int);
		void setpos(coord);
		//!@}

		//!@{ Getter functions
		int getheight();
		int getwidth();
		int getlines_scrolled();
		coord getpos();
		coord getcorner_top_left();
		//!@}
		
		void display();	//!< Display the layout
		void hide(); //!< Hide the layout
		void clear(); //!< Deletes contents of the layout
};

//! Represents a border
/*!
 Basically represents a border with characters that can be
 customised to suit the requirements.
*/
class frame
{
	char border_chars[8];	//!< chars used to draw border
	int tcolor;				//!< text color
	int bcolor;				//!< background color
	
	//! Represents what part of frame is visible.
	int sides_visibility[8];
	int frame_visibility;	//!< Frame visible or not
	coord corner_top_left;  //!< coord of top left corner

	//!@{These include the border characters too
	int height;  			//!< height
	int width;				//!< width
	//!@}

	//! Internal pmt used by operator<<
	int state;

	//! Sets the visibility of the side
	/*!
	 /param side Specifies the side using ui::dir
	 /param visib Set the visibility of the side
	*/
	void setside_visibility(int side, int visib);

	//! Converts the ui::dir code into internally usable code
	int convert(int);

	//! Prints the frame
	/*!
	 /param f_visib If 1, frame is printed; hidden if it's 0
	*/
	void print(int f_visib = 1);

	public:

		//! Used to set the visibility mode of the frame
		/*
		 all: -----
		 	  |   |
			  -----
		 nosides: ------

		 		  ------
		*/
		enum visibility_modes
		{
			all = 1,
			nosides = 2
		};
		
		//! Ctor
		/*!
		 /param corner_top_left Top left corner of frame
		 /param width Width of the frame
		 /param height Height of the frame
		*/
		frame(coord corner_top_left = coord(1,1), int width = 
		ui::scr_width, int height = ui::scr_height - 1);

		void display();	//!< Display the frame
		void hide();    //!< Hides the frame
		
		//! Sets the visibility mode of the frame
		void setvisibility_mode(int);

		//!@{ operator<<
		frame & operator<<(int); //!<Sets state

		//! Sets border_char according to state
		frame & operator<<(char);
		//!@}

		//!@{ Getter functions
		int getheight();
		int getwidth();
		coord getcorner_top_left();

		//! Returns 1 if visible; 0 = not visible
		int getframe_visibility(); 
		int gettcolor();
		int getbcolor();
		char getborder_char(int);
		int getside_visibility(int);
		//!@}

		//!@{ Setter functions
		void setheight(int);
		void setwidth(int);
		void settcolor(int);
		void setbcolor(int);
		void setcorner_top_left(coord);
		//!@}
};

//! Info related to a text box
/*!
 Stores information related to a text box
 Such as what type to convert it's data to
 and where to store it
*/
struct info_tbox
{
	text_box * tbox;	//!< ptr to text_box whose info is stored
	
	//! Data type to convert the string stored in text box to
	int type;
	void * data_store;	//!< Where to store converted data

	/*!
	 A validation function that's used to validate the
	 string stored in the text box to see if it is of
	 the required type before converting it.
	 /param str The string to validate
	 /param return 1, if string is validate; 0, otherwise
	*/
	int (*validator)(const char *str);

	//! The data types the string stored in text box represents
	/*!
	 Whenever a text box is set, the pointer to the place where
	 final data has to be stored is converted to a void* and
	 the data type is stored.
	 So, void* in different cases is:

	 data type     | What void* was
	 ------------- | --------------------
	 INT		   | int *	 
	 LONG		   | long *
	 UNSIGNED_LONG | unsigned long *
	 STRING		   | char *
	 CHAR		   | char *
	 DOUBLE		   | double *
	 FLOAT		   | float *
	 PASSWORD	   | char *
	*/
	enum data_types
	{					
		INT, 			
		LONG,			
		UNSIGNED_LONG,  
		STRING,			
		CHAR,			
		DOUBLE,			
		FLOAT,			
		PASSWORD,		
		OTHER //!< Not supported at the moment
	};

	info_tbox();	//!< Ctor

	//! Sets data to the data_store
	/*!
	 Gets the string stored in the text box, validates
	 it using the validation function and then converts
	 the string to the required data type and stores it in
	 the required space
	 /return 1 on success, 0 on invalid data
	*/
	int setdata();
};

/*!
 Contains default validation functions of type
 int f(char *)
 that take in a string and return 1 if the string
 is valid and 0, otherwise
*/
class validation
{
	validation(); //!< Object of this class is not allowed
	public:	

		//!@{ Default validation functions
		static int vint(const char *);
		static int vlong(const char *);
		static int vunsigned_long(const char *);
		static int vstring(const char *);
		static int vchar(const char *);
		static int vdouble(const char *);
		static int vfloat(const char *);
		//!@}

		/*!
		 Get the default validator function for the type
		 specified. If func is not NULL, returns default
		 function, else returns v
		*/
		static validator_f getvalidator(int type,
								 validator_f func);
};

/*!
 Represents a line with the three strings depiciting
 left, middle and right aligned stuff respectively
*/
struct line
{
	//!@{ Parts of the line
	char left[100];	  //!< left aligned
	char middle[100]; //!< centre aligned
	char right[100];  //!< right aligned
	//!@}

	int width;	//!< width of line
	int tcolor; //!< text color
	int bcolor; //!< background color
	coord corner_top_left; //!< coord of top left corner

	line(); //!< Ctor
	void display(); //!< Display the line
	void hide();	//!< Hide the line
	void clear();	//!< Delete the data stored

	private:
		void print(int); //!< Print the line according to arg
};

/*!
 Default Back function for use in the class box.
 Can't declare it as member function as member functions
 are not inherently addresses and setting it as a member function
 was causing unsolvable problems
*/
int default_back_func();

//! A box that has a border and a layout
/*!
 Basically incorporates all the elements into a single
 entity that the user will interact with.
 Basically looks like
 ------------------  <-- Frame
 | -------------- |
 | |		  <-------Layout (No border)	
 | |			| |	
 | -------------- <----Padding (between layout and frame)
 ------------------
*/
class box
{
	int height;		//!< Height of the box
	int width;		//!< Width of the box
	int padding;	//!< Padding between frame and layout

	/*!
	 Wraps a string with specified number of characters 
	 in each line
	 /param str String to wrap. Will be modified
	 /param length Number of chars in a line
	 /param return_one_line Sets string to have only one line
	 /return Number of lines after wrapping
	*/
	int wrap(char str[], int length, int return_one_line = 0);

	//! Sets the tbox
	/*!
	 Sets the textbox in the layout and also stores the 
	 correpsonding data in a tbox that is stored in the array
	 /param data_type Type of data in text box
	 /param ptr Pointer to the data store to set in tbox
	*/
	void set_tbox(int data_type, void *ptr);

	//!@{ Lists of interactives and text boxes
	interactive * list_interactive[30]; 
	info_tbox list_tbox[30];
	int index_interactive; //!< Index of element to set next
	int index_tbox; //!< Index of element to set next
	//!@}

	//! Clicking this button exits the loop
	button * exit_btn;

	//!@{ Toggles that help setting required info in layout
	int center_toggle;
	int default_toggle;
	int right_toggle;
	int header_toggle;
	int footer_toggle;
	int password_toggle;
	//!@}

	char default_text[100]; //!< Default text to set in textbox

	/*!
	 A temporary variable that stores validator func till it
	 is stored in the required place.
	*/
	int (*temp_validator)(const char *);

	//!@{ Header and footer
	line header;
	line footer;
	//!@}

	/*!
	 The function is called when the user performs a back func
	 while interacting with any interactive
	 /return 1, if loop exits on back; 0, if it does nothing
	*/
	int (*back_func)();

	protected:
		coord pos_pointer;	//!< Pos of pointer in box
		list_layout layout; //!< Layout in which data is stored
		coord corner_top_left; //!< Coord of top left corner

	public:

		//!@{ Manipulators can be used to alter function of <<
		static manipulator setheader;
		static manipulator setfooter;
		static manipulator setpassword;
		//!@}

		frame f;	//!< Border of the box

		//! Ctor
		/*!
		 Initialises all the variables of the class
		 /param corner_top_left The top left corner
		 /param width Width of box (includes border)
		 /param height Height of box (includes border)
		*/
		box(coord corner_top_left = coord(1,1), 
			int width = ui::scr_width,
		 	int height= ui::scr_height - 1);

		//!@{ Getter functions
		coord getcorner_top_left();
		int getheight();
		int getwidth();
		int getpadding();
		//!@}

		//!@{ Setter functions
		void setcorner_top_left(coord);
		void setheight(int);
		void setpadding(int);
		void settcolor(int);
		void setbcolor(int);
		void settcolor_selected(int);
		void setbcolor_selected(int);
		void settcolor_input(int);
		void setbcolor_input(int);
		void setback_func( int(*f)(void) );
		//!@}

		//!@{ operator<< is used for adding data to the box's
		//!	  layout that will be printed
		box & operator<<(char *);
		box & operator<<(char);
		box & operator<<(int);
		box & operator<<(long);
		box & operator<<(unsigned long);
		box & operator<<(double);
		box & operator<<(float);
		box & operator<<(manipulator);
		//!@}

		//!@{ operator>> is used for basically setting a text
		//!   box at the place where pos_pointer is currently
		//!	  at
		box & operator>>(char *&);
		box & operator>>(char &);
		box & operator>>(int &);
		box & operator>>(long &);
		box & operator>>(unsigned long &);
		box & operator>>(double &);
		box & operator>>(float &);
		box & operator>>(manipulator);
		
		//! Using this before another >> will set this func
		//! as the validator of that text box
		box & operator>>(int (*)(const char *));
		//!@}

		void setexit_button(char *);

		//!@{ Sets default for the next text box and
		//!   clears it after the next text box has been
		//!   set
		void setdefault(char *);
		void setdefault(char);
		void setdefault(int);
		void setdefault(long);
		void setdefault(unsigned long);
		void setdefault(double);
		void setdefault(float);
		//!@}

		/*!
		 Sets the box to loop, effectively enabling
		 all the text boxes and buttons. Also enables
		 scrolling
		*/
		void loop();

		void display();	//!< Display the box
		void hide();	//!< Hide the box
		void clear();	//!< Delete the contents of the box

		//!@{ Functions to set header and footer
		void setheader_tcolor(int); //!< set header color
		void setfooter_tcolor(int); //!< set footer color
		void clear_header(); //!< Delete contents of header
		void clear_footer(); //!< Delete contents of footer
		//!@}
};

#endif /* UI_HPP */
