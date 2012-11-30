SFMLLIB = -lsfml-graphics-d -lsfml-window-d -lsfml-system-d
OBJS = Game.o Actor.o Grid.o Player.o main.o Input.o
DEPS = Game.h Actor.h Grid.h Player.h Input.h

stratego : $(OBJS)
	g++ $(OBJS) -o stratego $(SFMLLIB)

%.o : %.cpp $(DEPS)
	g++ -c $< -o $@ 




