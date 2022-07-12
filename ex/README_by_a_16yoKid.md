## amGHOST V0.1a
[NOTE TO ANYONE WHO LIKES TO JUMP RIGHT INTO THE CODE.... JUST CHECK OUT amGHOST_System.hh First.... PLEASE.... Just My This 1 Req 2 U 🙏]

GHOST.... Generic Handy Operating System Toolkit, [Yes I'm Talking about Blender's GHOST 😄]
That is what inspired me to make my very own GUI API, Fow Now I Can only confirm the Support for windows. If anyone would be kind to then, I'm open to test results, feedback, small talks, pull requests, ideas, Collab

amGHOST means Amateur GHOST, why tho? Bcz, this isn't that much powerfull, This is just a speedup template kind of thing to just boost-start your Project if you need a GUI

The SOLE GOAL for this was to make a FASTER PERFORMING API but with LESS & MANAGABLE CODE which ANYONE CAN UNDERSTAND and CHANGE. Extended Explanation after HOW TO USE SECTION

WHERE DO YOU FIND ALL THE API USER FUNCTIONS?
amGHOST_System.hh has most of the Starting Functions in it.... like create_window(), process_events() while things like show_window(), hide_window(), openGL_create_context,() are inside amGHOST_Window.hh .... amGHOST_Context.hh has some openGL Functions too but mostly amGHOST_Window class has got the interesting ones

[STAY TUNED, MORE TO BE ADDED SOON.... 😉]

## PUBLICITY STUNT
YOU CAN CREATE A WINDOW with just 3 FUNCTION Calls.... See the Part I and PART II
Wait wait, We could have done it in 2... But Then GLFW Would Come running to be Saying WHY Did I Steal their CONCEPT of Doing it in 2 Function Calls
So, TOODLES!!!!
[THIS_IS_THE_MOMENT_WHERE_U_SAY_SUS]

## BUILD
Well, its as Simply as it Goes.... 
If you are NEW to CMAKE:    Check this one (+Links in it, LLVM-CmakePrimer) [https://gist.github.com/mbinna/c61dbb39bca0e4fb7d1f73b0d66a4fd1]
A Good Blog on PUBLIC/PRIVATE/INTERFACE by Lei Mao [https://leimao.github.io/blog/CMake-Public-Private-Interface/]
in CMAKE You don't Have to Know it All,    Whenever you come Across new Stuffs its PRETTY EASY to understand Really
You can Try this CMakeLists.txt too


## HOW TO USE? [PLEASE DON'T BE FREAKED OUT BY SEEING THIS MANY PARTS]
0. If you Read-Along Everything That Would be Easier to Understand and Faster. I'm Working on Making a Simple Youtube-Kinda-Introduction-Video what They like to call a TUTORIAL these days.... [Man, I HATE That Word.... ]. Also Remember to Check PART VI, pls....

1. Part I, Create the amGHOSTSystem
- #include "amGHOST_System.h" and add the path to amGHOST Folder in CMAKE or if you ain't using CMAKE, include full path/whatever your IDE/CmakeAlt needs
- Call amGHOST_System::create_system()
- Call amGHOST_System::get_system() [returns the created system, lets say we store that in amg_sys]
- CONGRATS, You've Succesfully added amGHOST to whatever it is that you are making. Just Remember to link the amGHOST.lib or if you are using CMake and want to Build amGHOST Yourself, then CMAKE add_subdirectory(PATH/TO/amGHOST)

2. Part II, Create an EK
- Create an EKFunction a.k.a Event Consumer Function, [int EKFuncName(amGHOST_Event *evnt);] Inside of this Function DO Whatever you want to do Based on that INPUT EVENT
- Add EK with amg_sys->add_EventKonsument(&EKFuncName); [MULTILE_EK_SUPPORTED]
- What is an EK? Event Konsument, Blender used to call it CallbackEventConsumer, and Konsument is Consumer, except for it's German version. 
- EKs are the functions that will be called [passing the amGHOST_Event as Parameter] for every Event when you wil call amg_sys->dispatchEvents();

3. Part III, Call <Functions-That-Made-You-Think-About-amGHOST-InThe-First-Place>
- EG. amGHOST_Window* wONE = amg_sys->create_window("WindowName", 0, 0, 1920, 1080, true);
- Remember we stored what get_system Returned in amg_sys. 

4. Part IV, THE MAIN-LOOP   [See-PartVIII-For-More]
- call amg_sys->process_events(true); and amg_sys->dispatch_events(); inside a Never Ending While loop.
- OR, You can Find an alternative than using NEVER ENDING WHILE LOOP, Given the Info, 
- "dispatch_events() Will return false if any of your EKFunction returns -1 over any Event" 😉
- [AND You know you could call stdlib exit() inside of EKFunction if you want your program to Exit based on some Event]

5. PART V, THE RULES & DETAILS:-
- You don't need to Manually do sleep inside MAIN-LOOP. That can cause some serious Issues, sometimes...
- WindowShouldClose is a EVENT rather than a Function
- From now on every amGHOST function call should be in-relation to CLASS Instances, like this amg_sys->openGL_create_context(), wONE::hide_Window()
- NOTE. amg_sys is an amGHOST_System instance, and the "wONE" that, create_window() returned is also a amGHOST_Window instance
- This all boils down to this..., that you should not try to instantiate any amGHOST Class itself. But the fact is, in some cases like amGHOST_EK, amGHOST_Event, You can instantiate.... But why do so?

6. PART VI, THE WARNINGS COME AFTER THE SPELLS 😏 [TRICKS-DOES-TOO!]
- 0: Remember that amGHOST_System is an Abstract Class, meaning that it contains PURE VIRTUAL Functions, that's why you can't instantiate amGHOST_System
- 1: If you have tried out amGHOST, you should recognize that Every Abstract Class like amGHOST_System, amGHOST_Context, amGHOST_Window, they each have Platform Specific Files and the thing is amGHOST_System internally handles those distrinctions

- 2: amGHOST_System::get_system returns [amGHOST_System*] so you'll have to use '->' instead of '::' calling functions relative to amg_sys
- 3: BEWARE amGHOST_System Includes amGHOST_Types.h, amGHOST_Context.h, amGHOST_Event.h, amGHOST_EK.h

- 3: Make sure that you don't Return -1 from any of the EKFunction that you make. But please also Make sure to Return something

- 4: Before you Exit your program remember to call amGHOST_System::dispose_system()
Check out the test.cpp and test.c inside test folder

- 5: Following OpenGL's Recent Feature GLDebugMessageCallback, we implemented that feature too, [😉]
you can call add_errorKonsument(&FuncName); and the function should look like this for now [void FuncName(char *ErrorStringInfo);]


7. PART VII, MULTI-THREADING
- Heard of the idea of UI-THREAD? almost 90% Would agree that ther should not be more than 1 UI-THREAD
- Also the fact that there can't be more than 1 amGHOST_System doesn't really give any advantage for using amGHOST over multiple-Threads except for DIFFERNT Windows througout different Threads. The Support for that Will be Added SOON [TODO]


8. PART VIII, MAIN-LOOP Details [PART IV Extended]
- process_events() processes all the os inputs and converts them into amGHOST Events then adds them to a EventQueue [std::deque type]..
- dispatch_events() passes event to all the EKs one by one from the EventQueue, and if Any EK returns 0, dispatch_events immediately Returns false.. But yes the rest of the [Events except the one that caused EK to return -1], are still there in the Queue
- We Let you decide to return -1 based on INPUTS, because you might want to implement a YES/NO Confirmation message if Someone Hits the Cross Button or Close the WIndow from System TaskBar/Tray
- ______________________________
- ---------- DETAILS -----------
- If you are not Familiar with the Main-Loop concept, then Let me introduce you to the Idea of it....
- You are making an GUI right? And you want to Act on Mouse Clicks, Keys, Buttons, right? If so, then you need to always be-aware Events from users of your program
- By Events we mean, Mouse/Key/Button or any kind of Interactive Device inputs even VR, AR...
- The fact that we always to check for inputs if what introduced the idea of MAIN-LOOP
- Some may ask, Don't we always need to check for incoming inputs for instant Interactions, Wouldn't that use 100% CPU? NO!!!!, there are workarounds. Like Letting the Program go into Sleep mode and if any Input is received, then the HARDWARE Will respond to OS and the OS will AWAKE amGHOST
- SO You can call amg_sys->process_events(true); the parameter 'true' means to Go to Sleep if there are no incoming inputs at the OS Level and till any INPUT is Receieved program is simply just sleeping, not using CPU Core/Thread. and process_events functions will return as_soon_as it's awaken by any kind of Input [from_OS]


## WHY NOT JUST USE GLFW????
But Still yet amGHOST:-
- You don't need to know what GLEW is... 
- You don't need to think about Init functions like glew_init() and glfw_init() amGHOST Handles that internally
- GLFW Promotes glfw_pollEvents() from the Very beginning of Time [1999], They added glfw_waitEvents() in 2.5 [2005] in their EVENT PROCESSING Docs page, They do say waitEvents is Better. But the Implementation of that Function is Not that Much revealed.... SO I always Felt like GLFW is Shady for Silly Reasons Like this.... AND.... WE DON'T DO THAT HERE....
- calls Less Functions.... I really tried to make it as compact as possible
- will be more Fun to discover, jokes and notes are just Floating everywhere
- Functions are Organized in the Top-Level Header FIles like amGHOST_System.hh amGHOST_Window.hh. They are Also Categorized
- I really tried to NOT USE any PARTICULAR STYLE or WAY OF THINKING.... 
- - The Only thing Functions here return is a POINTER TO A CLASS like [amGHOST_Window*, amGHOST_System*, amGHOST_Context*] 
- - and just simple BOOL or amGHOST_TSuccess [based on amGHOST_RETURN_BOOL macro]
- - or IF You are using C-API then C-api Functions return Data Types from amGHOST_Types.h which are also Compact
- - SO NO UNnecessary Pointer or Handles
- - What About Code Writing STyle? Well You know, in Open Source Development You can't really NEVER have same styled CODE in a BIG PROJECT
- - What about the fact that you just created decided to use bunch of classes? - I mean, come'n It Organizes Things furthur like nothing Else on the planet can
- - What about the Fact that You just implemented most of the things inside a Type,, some of which could be done with preProcessor
- ------------------------
- JUMP INTO THE CODE and START EXPLORING, Good Starting Point is amGHOST_System.hh and intern/amGHOST_System.cpp
- AND KEEP ADDING YOU VERY OWN FEATURES


## WEIRD SHITS GOING ON [a.k.a WEIRD Concepts/Features]
1. EK.... a.k.a EVENT KONSUMENTS a.k.a CallbackEventConsumer a.k.a Functions that are invoked when you call dispatch_events one by one. ALL EKs are called for every event till there is no event in EVENTQ, then dispatch_events Return. Yes, You can have multiple EKs

2. If EK returns '-1' then that causes dispatch_events to immidiately return false removing that event from the EventQ. This also means, if you have 3 EKs and 2nd EK returns -1 for some reason while responding to a particular INPUT EVENT, then the 3rd EK That you created won't be called with that event ever

++ More Return Code OF EK to be introduced soon....[TODO]


## HELP ME [I mean, I you are Interested in Development 😅]
FEEL FREE TO CHECK OUT NOTES.txt for now and JUST EXPLORER, I am Pretty Sure you will catch up with the Code pretty FAST!!!!


## ACKNOWLEDGEMENT
All thanks to Blender and Blender's GHOST, really.... This is an Complement re-write of that. 
They have a Lot of Helping Docs in their code.... and It was all possible because Blender is again... OPEN-SOURCE as Always

## License
amGHOST is licensed under GNU GPL 3.0
See license.txt