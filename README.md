# Scarab-Engine version 1.0 release 004
  
  A Simple and Complete Architecture for Rendering, Animating and Building virtual environments.

  by Remi "Shikifuyin" Lefevre


 - Changelog :

                          v1.0r004 (07/01/2015) 

    Major update I have been working on for the last few months after being off for a while.
    No demo this time, I'm working on something much bigger now which should pack quite a punch !
    However, given the huge amount of work involved with that I won't give any promise on a
    alpha/beta release just yet !

    => Rendering : Complete rewrite of the rendering engine & effects APIs, DirectX 11 support implementation.
                   No more dependency with old DirectX 9 SDKs. Windows 7 is now the minimal requirement.
                   Improved debug rendering.
                   Partial rewrite of the outdoor visibility algorithm, made it much more generic and is now
                   independant of world hierarchy/representation complexity.

    => Physics : V-Clip is now much more stable, and should be the default algorithm now.
                 SAT implementation for collision detection, still needs testing, kinda unreliable ...
                 GJK-EPA / GJK-MPR implementation for collision detection, very promising, ... weak for contact caching though !
                 Improved contact point generation & caching.
                 Lots of corrections and debugging on the constraint solver, added rolling friction support.
                 All joints are now fully operational with support for generic limits, (servo)motors and springs.
                 Numeric & geometric stability improvements.

    => GUI : Lots of bug fixes, major performance gains for 2D and text rendering.
             Added ComboList widget.

    => Misc : Overall code cleanup and bug fixes.
              Various bug fixes in library, maths in particular.
              
                          v1.0r003 (12/06/2013)

 => Resources : Added XML support for future COLLADA resources.
 => Physics : Finished kinematic responses integration.
              Added character controller, to be improved further.
 => World : Added path-finding support.

 => SimpleRPG demo : Added kinematics and path-finding, various
                     improvements.

 => Lots of bug fixes.

                          v1.0r002 (21/05/2013)

 => Rendering : Improved 2D & Font batching capacities (multiple texture channels)
 => GUI : Added event bypass/redirect capabilities (non-blocking windows)

 => Various Engine improvements and optimizations.
 => Various bugfixes.

 => Removed scratch entry point, replaced with SimpleRPG Demo (in development)

                          v1.0r001 (06/02/2013)

 => Initial release ...


 - Installation :

You need to have Microsoft Visual C++ 2012 installed.
OpenGL is not yet supported ... (will come quite soon).

Decompress the archive in your favorite project folder.
In Visual C++, load "_VCProject/Scarab Engine/Scarab Engine.sln".

You have full access to the code this way, but the project can also
be used as a set of static libraries for redistributable packages.

 - WebSite :

https://www.scarab-engine.net/ (DOWN FOR REFACTORING)

Here you will find latest releases, patch-notes, the documentation,
and a community forum.

Sorry the documentation is almost empty for now ! This is a lot of
work and I'll try to fill it as fast as possible ...
You should read the "Getting Started" section to begin with. The
"Coding Conventions" and "Basic Framework" sections will be available
very soon ...

You can now start to experiment with the engine !
It should compile without any error and just a few warnings on code
that is either in development or scheduled for refactoring.
You should allways compile with warnings at level 4 at least (or Wall).
When debugging, don't forget to look at the output and log files for
informations about failed assertions.

Feel free to contact me for any help needed using the forum or the
project mailbox at scarab.engine@gmail.Com ...


 - License :

http://creativecommons.org/licenses/by-nc-nd/4.0/

The Scarab-Engine project is licensed under the
Creative Commons Attribution-NonCommercial-NoDerivs 4.0 World-wide License.

This license applies to all source files and associated resources, including
the website itself.

Modifications and evolutions can be allowed using the website's forum,
I'm open to all of your suggestions and I'll take interest in including
new ideas and work to this project.
Authors will of course be mentionned for their work according to the
license and added to the credits page.
Beware though I'm very picky with code conventions and I won't hesitate
to refactor suggested code to fit with the project's conventions, still
giving full credits to the code author.
Lastly, these must NOT concern work that is either in the working or
future work lists since I'm already working on it. Also I'll take a
special care in avoiding redundancies and gathering people to work
together for similar, related or inter-dependant ideas.


    Thank you for downloading and using my engine !
