#include<ncurses.h>
#include<stdlib.h>


typedef struct {
			WINDOW  *w;		
			int width,startx,starty;		//startx & starty give the positions of the disc 
			} disc;			



typedef struct {	char name;
			int top,x;		//top corresponds to the top y-coordinate of towers where new disc can be put
			} peg;		// x corresponds to the x-coordinate of peg


						//Create an array whose array index corresponds to the disc no.
disc z[25];					//     1 is the smallest disc & n is the biggest disc      */
peg a,b,c;			
peg *A=&a,*B=&b,*C=&c;
int HEIGHT=2 ,WIDTH=26 , BASE=0 , sleep , j ,i=1 ,up=11,p=0;
float unit;
WINDOW *new,*fill;

const int SPEED = 3000;

WINDOW *create_newwin(int height, float width, int starty, int startx);
void destroy_win(WINDOW *local_win);
void hanoi(int n,peg *S,peg *D,peg *E);
void shift(int n,peg *S,peg *D);
int steps(int n);




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



int main(int argc, char *argv[])		//Beginning of MAIN function
	{			
		int i,n;
			
		initscr();
		start_color();			//Starting the colour mode
		raw();
		keypad(stdscr,TRUE);		
		BASE=LINES-4;			//BASE refers to the y-coordinate of the ground on which the discs are  positioned.
	
		a.top=BASE-HEIGHT ; a.x=1+(WIDTH/2) ; a.name='A';		//Initialising the pegs' attributes
		b.top=BASE-HEIGHT ; b.x=COLS/2 ; b.name='B';
		c.top=BASE-HEIGHT ; c.x=COLS-(WIDTH/2)-1 ; c.name='C';
	
		mvprintw(0,(COLS/2)-4,"Enter n:");		//Taking input 'n' from user
		refresh();
		attron(A_BOLD);
		scanw("%d",&n);
		attroff(A_BOLD);
		if(n>=13)  {			mvprintw(5,COLS/2-11,"ERROR : Invalid Entry.");	//Input validation
							refresh();
							getch();
							endwin();
							exit(0);					
						   }	
	
		unit=(float)(COLS-24)/(float)steps(n);	//unit of increment of fill 'window'
		p=n;							//p assigned value of n to be used by other functions
	
	
		mvprintw(1,(COLS/2)-20,"Enter speed(1=fast,2=med,3=slow) :" );
		refresh();					
		attron(A_BOLD);				//Input speed of execution
		scanw("%d ",&sleep);
		curs_set(0);
		if(sleep!=1 && sleep!=2 && sleep!=3) 	//Input validation
			 {	mvprintw(5,COLS/2-11,"ERROR : Invalid Entry.");	
				refresh();
				getch();
				endwin();
				exit(0);					
		         }	
		
		
		new=create_newwin(LINES,COLS, 0,0);	//A new window is created to give the border effect
		
		mvprintw(1,(COLS/2)-24,"Solution of 'TOWERS OF HANOI' problem for n = %d",n);//Print headline
		
		for(i=a.top;i>=up;i--)	mvprintw(i,a.x,"|");		//Printing Peg A
		for(i=b.top;i>=up;i--)	mvprintw(i,b.x,"|");		//Printing Peg B
		for(i=c.top;i>=up;i--)	mvprintw(i,c.x,"|");		//Printing Peg C
		
		for(i=2;i<=COLS-3;i++)  mvprintw(up-2,i,"_");           //Printing uper division line
		for(i=2;i<=COLS-3;i++)  mvprintw(BASE,i,"_");         //Printing base line
		mvprintw(BASE+2,a.x-3,"Peg  A ");			//Writing name of Peg A	
		mvprintw(BASE+2,b.x-3,"Peg  B");				//Writing name of Peg B
	 	mvprintw(BASE+2,c.x-3,"Peg  C"); 				//Writing name of Peg C
		refresh();	
		
		
	
		for(i=n;i>=1;i--) //Loop for printing the discs on PEG A.No. of times loop runs = n
		       {
				z[i].width=WIDTH-2*(n-i) ;			//Assigning width to each disc
				z[i].w=create_newwin( HEIGHT , z[i].width=WIDTH-2*(n-i) , z[i].starty=a.top , 
								z[i].startx=a.x-(z[i].width)/2);	//Printing discs as windows	
				init_pair(1, COLOR_YELLOW, COLOR_BLACK);attron(COLOR_PAIR(1));
				mvwprintw(z[i].w,1,(z[i].width)/2,"%d",i);		//Printing disc numbers
				attroff(COLOR_PAIR(1));
				wrefresh(z[i].w);
				a.top=a.top-HEIGHT;				//Re-adjusting the height of Peg A
			}//Printing loop ends here
		
		fill=create_newwin(3,COLS-19,5,16);			//Creating new status window 
		getch();	
		attroff(A_BOLD);
		
		hanoi(n,A,C,B);							//Calling HANOI function to solve the problem for 'n'
		attron(A_BOLD);		attron(A_STANDOUT);
		mvprintw(LINES/2,(COLS/2)-18,"SUCCESS!!!  Press any key to EXIT....");       //Print exit message
		attroff(A_STANDOUT);	attroff(A_BOLD);
		refresh();
		getch();
		endwin();
		return 0;
		
	} //MAIN function ends here
	


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	


void shift(int n,peg *S,peg *D)	//Function for shifting a disc from Peg S(source) to Peg D(Destination) starts here
		{	
			init_pair(2, COLOR_RED, COLOR_BLUE);attron(COLOR_PAIR(2));
			mvprintw(3,(COLS/2)-32,"CURRENT STATUS ");		//Printing CURRENT  STATUS in colour
			attroff(COLOR_PAIR(2));
			init_pair(3, COLOR_GREEN, COLOR_BLACK);attron(COLOR_PAIR(3));
			mvprintw(3,(COLS/2)-15,": Disc %d is being shifted from Peg %c to Peg %c",n,(*S).name,(*D).name);
			attroff(COLOR_PAIR(3));
			init_pair(2, COLOR_RED, COLOR_BLUE);attron(COLOR_PAIR(2));
			mvprintw(6,2,"PROGRESS : ");					//Printing PROGRESS in colour
			refresh();
			attroff(COLOR_PAIR(2));
			for(j=(*S).top;j>=up;j--) 
							{ 	//Loop for moving a disc upwards      
								destroy_win(z[n].w);
								z[n].w=create_newwin(HEIGHT,z[n].width,j,(*S).x-(z[n].width)/2);
								mvwprintw(z[n].w,1,(z[n].width)/2,"%d",n);
								wrefresh(z[n].w);		
								mvwprintw(new,j+2,(*S).x,"|");  //Keeping track of peg 
								wrefresh(new);
								usleep(3*(2*sleep-1)*SPEED);	//Sleep command to monitor speed
						   	 }
	
			if((*S).x<(*D).x)    
				{		//Loop for moving disc towards right
						for(j=(*S).x;j<=(*D).x;j++) 
							{	
								destroy_win(z[n].w);
								z[n].w=create_newwin(HEIGHT,z[n].width,up,j-(z[n].width)/2);		
								mvwprintw(z[n].w,1,(z[n].width)/2,"%d",n);
								wrefresh(z[n].w);			
								usleep((2*sleep-1)*SPEED);	//Sleep command to monitor speed	
						      }
				  }//Termination of 'if' condition 
			

			else                   
				  {		
						for(j=(*S).x;j>=(*D).x;j--) 
							{	//Loop for moving disc towards left
								destroy_win(z[n].w);
								z[n].w=create_newwin(HEIGHT,z[n].width,up,j-(z[n].width)/2);		
								mvwprintw(z[n].w,1,(z[n].width)/2,"%d",n);
								wrefresh(z[n].w);		
								usleep((2*sleep-1)*SPEED);	//Sleep command to monitor speed	
							 }
				   }//Termination of 'else' condition 
	
			for(j=up;j<=(*D).top;j++)   
							 {    	  //Loop for moving disc downwards
								  destroy_win(z[n].w);
								  z[n].w=create_newwin(HEIGHT,z[n].width,j,(*D).x-(z[n].width)/2);		
							       	  mvwprintw(z[n].w,1,(z[n].width)/2,"%d",n);
								  wrefresh(z[n].w);		
								  if(j>(up+2)) {mvwprintw(new,j-1,(*D).x,"|");	//Keeping track of peg 
								  wrefresh(new);}
								  usleep(3*(2*sleep-1)*SPEED);	//Sleep command to monitor speed
						   	  }
			
	
			 (*D).top-=HEIGHT;				//Re-adjusting the height of destination Peg
			
			if((*S).top!=BASE)  (*S).top+=HEIGHT;  //Special case when initially disc is at the base
			
			init_pair(4, COLOR_GREEN, COLOR_MAGENTA);attron(COLOR_PAIR(4));
			mvprintw(8,(COLS/2)-6,"%d %% complete",(i*100)/steps(p)); //Printing the progress %age in colour
			attroff(COLOR_PAIR(4));
			
			init_pair(1, COLOR_BLACK, COLOR_YELLOW);attron(COLOR_PAIR(1));
			for(j=18;j<=18+(float)i*unit;j++)	{mvprintw(6,j," ");  refresh();}
			attroff(COLOR_PAIR(1));		//Printing the progress filling in colour
			i++;						//Incrementation of loop variable
		
		}	//SHIFT function ends here
	


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	

void hanoi(int n,peg *S,peg *D,peg *E)	//HANOI problem solving function is declared here
		{					//It shifts n discs from Peg S(source) to D(destination) via Peg E(extra)
			if(n>0)//Functionm runs while n>0
				{
				hanoi(n-1,S,E,D);	//Shifting (n-1) discs from S to E
				shift(n,S,D);		//Shifting 'n' th disc from S to D
				hanoi(n-1,E,D,S);	//Shifting (n-1) discs from E to D
				}
		}//HANOI function ends here
	

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


WINDOW *create_newwin(int height, float width, int starty, int startx)//Function for creating a window 
	{	WINDOW *local_win;
		local_win = newwin(height, width, starty, startx);
		box(local_win, 0 , 0);			
		wrefresh(local_win);
		return local_win;
	}	//WINDOW CREATING function ends here
	

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void destroy_win(WINDOW *local_win)//function for destroying a window
	{	
		wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
		wrefresh(local_win);
		delwin(local_win);
	}	//DESTROY WINDOW function ends here
	

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int steps(int n)	//function for counting the no  of steps involved
	{
		if(n==1) return 1;
		else return (2*steps(n-1)+1);
	}	//Counting function ends here
	

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			

							//THE END//
