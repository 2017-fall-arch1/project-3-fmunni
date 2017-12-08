 
/** \file shapemotion.c
 *  \brief This is a simple shape motion demo.
 *  This demo creates two layers containing shapes.
 *  One layer contains a rectangle and the other a circle.
 *  While the CPU is running the green LED is on, and
 *  when the screen does not need to be redrawn the CPU
 *  is turned off along with the green LED.
 */  
#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include <shape.h>
#include <abCircle.h>
#include <string.h>
#include "buzzer.h"



#define GREEN_LED BIT6



//AbRect rect10 = {abRectGetBounds, abRectCheck, {10,10}}; /**< 10x10 rectangle */
AbRect rect = {abRectGetBounds, abRectCheck, {7,3}};

short Pnt =1;
char player1Score = '0';
char player2Score = '0';
//static int state =0;
u_int player1count =0;
u_int player2count =0;

AbRectOutline fieldOutline = {	/* playing field */
  abRectOutlineGetBounds, abRectOutlineCheck,   
  {screenWidth/2 - 10, screenHeight/2 - 10}
  
};



Layer fieldLayer = {		/* playing field as a layer */
  (AbShape *) &fieldOutline,
  {screenWidth/2, screenHeight/2},/**< center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_BLACK,
  0
  //&layer3;
};


Layer layerB = {		/**< Layer with an orange circle */
  (AbShape *)&circle4,
  {(screenWidth/2), (screenHeight/2)}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_VIOLET,
  &fieldLayer,
  
};


Layer layerR = {		/**< right rectengle */
  (AbShape *)&rect,
  {screenWidth/2, (screenHeight/2)}, /**< center */
  {0,0}, {60,140},				    /* last & next pos */
  COLOR_RED,
  &layerB,
};

Layer layerL = {		/**< left rectangle */
  (AbShape *)&rect,
  {screenWidth/2+10, (screenHeight/2+5)}, /**< bit below & right of center */
  {0,0}, {45,15},				    /* last & next pos */
  COLOR_ORANGE,
  &layerR,
};

/** Moving Layer
 *  Linked list of layer references
 *  Velocity represents one iteration of change (direction & magnitude)
 */
typedef struct MovLayer_s {
  Layer *layer;
  Vec2 velocity;
  struct MovLayer_s *next;
} MovLayer;

/* initial value of {0,0} will be overwritten */
MovLayer mlB = { &layerB, {4,4}, 0 }; /**< not all layers move */
MovLayer mlR = { &layerR, {4,4}, 0 }; 
MovLayer mlL = { &layerL, {4,4}, 0 }; 


void movLayerDraw(MovLayer *movLayers, Layer *layers)
{
  int row, col;
  MovLayer *movLayer;

  and_sr(~8);			/**< disable interrupts (GIE off) */
  for (movLayer = movLayers; movLayer; movLayer = movLayer->next) { /* for each moving layer */
    Layer *l = movLayer->layer;
    l->posLast = l->pos;
    l->pos = l->posNext;
  }
  or_sr(8);			/**< disable interrupts (GIE on) */


  for (movLayer = movLayers; movLayer; movLayer = movLayer->next) { /* for each moving layer */
    Region bounds;
    layerGetBounds(movLayer->layer, &bounds);
    lcd_setArea(bounds.topLeft.axes[0], bounds.topLeft.axes[1], 
		bounds.botRight.axes[0], bounds.botRight.axes[1]);
    for (row = bounds.topLeft.axes[1]; row <= bounds.botRight.axes[1]; row++) {
      for (col = bounds.topLeft.axes[0]; col <= bounds.botRight.axes[0]; col++) {
	Vec2 pixelPos = {col, row};
	u_int color = bgColor;
	Layer *probeLayer;
	for (probeLayer = layers; probeLayer; 
	     probeLayer = probeLayer->next) { /* probe all layers, in order */
	  if (abShapeCheck(probeLayer->abShape, &probeLayer->pos, &pixelPos)) {
	    color = probeLayer->color;
	    break; 
	  } /* if probe check */
	} // for checking all layers at col, row
	lcd_writeColor(color); 
      } // for col
    } // for row
  } // for moving layer being updated
}	  



Region fence = {{10,30}, {SHORT_EDGE_PIXELS-10, LONG_EDGE_PIXELS-10}}; /**< Create a fence region */



/** Advances a moving shape within a fence
 *  
 *  \param ml The moving shape to be advanced
 *  \param fence The region which will serve as a boundary for ml
*/


void moveGame(MovLayer *ml, MovLayer *mlB, MovLayer *mlR, Region *fence)
{
    
    Vec2 newPos;
  u_char axis;
  Region shapeBoundary;
  for (; ml; ml = ml->next) {
    vec2Add(&newPos, &ml->layer->posNext, &ml->velocity);
    abShapeGetBounds(ml->layer->abShape, &newPos, &shapeBoundary);
    for (axis = 0; axis < 2; axis ++) {
      if ((shapeBoundary.topLeft.axes[axis] < fence->topLeft.axes[axis]) ||
	  (shapeBoundary.botRight.axes[axis] > fence->botRight.axes[axis]) ) {
	int velocity = ml->velocity.axes[axis] = -ml->velocity.axes[axis];
	newPos.axes[axis] += (2*velocity);
      
      
    
   // ml->layer->posNext = newPos;
}
      if ((ml->layer->posNext.axes[1]>=130)&&(ml->layer->posNext.axes[0]<=mlB->layer->posNext.axes[0]+20 && ml->layer->posNext.axes[0]>=mlB->layer->posNext.axes[0]-20))
      {
          int velocity = ml->velocity.axes[axis] = -ml->velocity.axes[axis];
          mlB->layer->color = COLOR_RED;
          mlR->layer->color = COLOR_ORANGE;
          ml->layer->color = COLOR_VIOLET;
          ml->velocity.axes[0]+=1;
          newPos.axes[axis]+=(2*velocity);
          
          int redrawScreen=1;
          //ml->layer->posNext=newPos;
    }
    
    else if((ml->layer->posNext.axes[1] <= 22)&&(ml->layer->posNext.axes[0] <= mlR->layer->posNext.axes[0] +20 && ml->layer->posNext.axes[0]>=mlR->layer->posNext.axes[0]-20))
        
    {
        int velocity = ml->velocity.axes[axis] = -ml->velocity.axes[axis];
        mlR->layer->color = COLOR_RED;
        mlB->layer->color - COLOR_ORANGE;
        ml->layer->color = COLOR_VIOLET;
        ml->velocity.axes[0] +=1;
        newPos.axes[axis] += (2*velocity);
        
        int redrawScreen =1;
    }
    
    else if((ml->layer->posNext.axes[1]==20)){
        mlR->layer->color = COLOR_RED;
        player1Score ++;
        player1count++;
        
        drawChar5x7(52,152,player1Score, COLOR_GREEN, COLOR_BLACK);
        newPos.axes[0] = screenWidth/2;
        newPos.axes[1] = (screenHeight/2);
        Pnt =1;
        ml->velocity.axes[0] =5;
        ml->layer->posNext = newPos;
        int redrawScreen =1;
    }
    
    //int redrawScreen =1;
    else if((ml->layer->posNext.axes[1] ==130)){
        mlB->layer->color = COLOR_RED;
        player2Score++;
        player2count++;
        drawChar5x7(120,152, player2Score, COLOR_GREEN, COLOR_BLACK);
        newPos.axes[0] = screenWidth/2;
        newPos.axes[1] = screenHeight/2;
        Pnt=1;
        
        ml->velocity.axes[0] =5;
        ml->layer->posNext= newPos;
        
        int redrawScreen =1;
    }
    int redrawScreen=1;
    if(Pnt != 1){
        ml->layer->posNext = newPos;
    }
    } /**< for axis */
    ml->layer->posNext = newPos;
  } /**< for ml */
}




u_int bgColor = COLOR_GREEN;     /**< The background color */
int redrawScreen = 1;           /**< Boolean for whether screen needs to be redrawn */

Region fieldFence;		/**< fence around playing field  */


/** Initializes everything, enables interrupts and green LED, 
 *  and handles the rendering for the screen
 */
void main()
{
  P1DIR |= GREEN_LED;		/**< Green led on when CPU on */		
  P1OUT |= GREEN_LED;

  configureClocks();
  lcd_init();
  shapeInit();
  
  p2sw_init(7);
  buzzer_init();
  

  layerInit(&layerB);
  layerDraw(&layerB);
  //layerInit(&layer0);
  //layerDraw(&layer0);

  

  layerGetBounds(&fieldLayer, &fieldFence);


  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x8);	              /**< GIE (enable interrupts) */

  u_int switches = p2sw_read();
  
  
  drawString5x7(3,152,"Red:", COLOR_RED, COLOR_GREEN);
  drawString5x7(72,152,"Orange:", COLOR_ORANGE, COLOR_GREEN);
  drawString5x7(52,152,player1Score, COLOR_WHITE, COLOR_GREEN);
  drawString5x7(120,152,player2Score, COLOR_WHITE, COLOR_GREEN);

  for(;;) { 
    while (!redrawScreen) { /**< Pause CPU if screen doesn't need updating */
      P1OUT &= ~GREEN_LED;    /**< Green led off witHo CPU */
      or_sr(0x10);	      /**< CPU OFF */
    }
    P1OUT |= GREEN_LED;       /**< Green led on when CPU on */
    redrawScreen = 0;
    
    movLayerDraw(&mlB, &layerB);
    movLayerDraw(&mlR, &layerR);
    movLayerDraw(&mlL, &layerL);
  }
}

/** Watchdog timer interrupt handler. 15 interrupts/sec */
void wdt_c_handler()
{
  static short count = 0;
  P1OUT |= GREEN_LED;		      /**< Green LED on when cpu on */
  count ++;

  
  if (player1count == 5 || player2count ==5) {
      
      bgColor = COLOR_GREEN;
      if(player1count ==5){
          GameWinner(0);
          layerDraw(&layerR);
          drawString5x7(screenWidth/2-38, screenHeight/2, " RED Player won.", COLOR_RED, COLOR_GREEN);
          
    }
    else{
        GameWinner(1);
        layerDraw(&layerR);
        drawString5x7(screenWidth/2 -38, screenHeight/2, "Yellow player won", COLOR_ORANGE, COLOR_GREEN);
    }
  } 
  
  
  if(count ==20){
      moveGame(&mlB, &mlR, &mlL, &fieldFence);
      //mlAdvance( &mlB, &mlR, &mlL, &fieldFence);
     u_int switches = p2sw_read();
      
      if(!(switches & (1<<1))){
          if(mlR.layer->posNext.axes[0]<=102){
              mlR.layer->posNext.axes[0] +=5;
              redrawScreen=1;
              Pnt =0;
        }
    }
    
    else if(!(switches &(1<<0))){
        if(mlR.layer->posNext.axes[0] >= 27){
            mlR.layer->posNext.axes[0] -=5;
            redrawScreen=1;
            Pnt=0;
        }
        
    }
    
    else if(!(switches &(1<<2))){
        if(mlL.layer->posNext.axes[0] >= 26){
            mlL.layer->posNext.axes[0] -=5;
            redrawScreen=1;
            Pnt=0;
        }
        
    }
    
    else if(!(switches &(1<<3))){
        if(mlL.layer->posNext.axes[0] <= 102){
            mlL.layer->posNext.axes[0] +=5;
            redrawScreen=1;
            Pnt=0;
        }
        
    }
    
    
    redrawScreen=1;
    count =0;
}		
/**< Green LED off when cpu off */

P1OUT &= ~GREEN_LED;
}


