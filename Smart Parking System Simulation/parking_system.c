#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h> // For sleep()

// --- Constants ---
#define MAX_FLOORS 4
#define SLOTS_PER_FLOOR 64
#define PRICE_PER_HOUR 5.0
#define PEAK_PRICE 8.0
#define OFF_PEAK_PRICE 3.0
#define LOG_SIZE 5

// --- Data Structures ---

// Car Structure
typedef struct Car {
  int id;
  time_t entry_time;
  struct Car *next; // For Queue
} Car;

// Queue (Entry/Exit)
typedef struct Queue {
  Car *front;
  Car *rear;
  int count;
} Queue;

// Floor Node (Circular Linked List)
typedef struct Floor {
  int floor_number;
  uint64_t slots; // 64 bits for 64 slots. 0 = Empty, 1 = Occupied
  struct Floor *next;
} Floor;

// Stack (Parking Levels - for LIFO evacuation/management)
typedef struct Stack {
  int items[MAX_FLOORS * SLOTS_PER_FLOOR];
  int top;
} Stack;

// Action Log
char actionLog[LOG_SIZE][100];
int logIndex = 0;

// Global Variables
Queue entryQueue;
Queue exitQueue;
Floor *currentFloorDisplay; // For rotating display
Floor *headFloor;           // Start of the circular list
Stack evacuationStack;
int carIdCounter = 1;

// --- Function Prototypes ---
void init_system();
void enqueue(Queue *q, int id);
int dequeue(Queue *q);
void push(Stack *s, int value);
int pop(Stack *s);
int find_free_slot(uint64_t slots);
void set_slot(uint64_t *slots, int bitIndex);
void clear_slot(uint64_t *slots, int bitIndex);
bool is_slot_occupied(uint64_t slots, int bitIndex);
void display_ui();
void process_entry();
void process_exit();
void simulate_emergency();
void add_car_to_entry();
void request_exit();
void log_action(const char *message);

// --- Implementation ---

void log_action(const char *message) {
  // Shift logs up
  for (int i = LOG_SIZE - 1; i > 0; i--) {
    strncpy(actionLog[i], actionLog[i - 1], 100);
  }
  strncpy(actionLog[0], message, 100);
}

void init_queue(Queue *q) {
  q->front = q->rear = NULL;
  q->count = 0;
}

void init_stack(Stack *s) { s->top = -1; }

void init_system() {
  init_queue(&entryQueue);
  init_queue(&exitQueue);
  init_stack(&evacuationStack);

  // Initialize logs
  for (int i = 0; i < LOG_SIZE; i++)
    strcpy(actionLog[i], "");

  // Create Circular Linked List of Floors
  Floor *prev = NULL;
  for (int i = 0; i < MAX_FLOORS; i++) {
    Floor *newFloor = (Floor *)malloc(sizeof(Floor));
    newFloor->floor_number = i + 1;
    newFloor->slots = 0; // All empty
    newFloor->next = NULL;

    if (i == 0) {
      headFloor = newFloor;
      currentFloorDisplay = headFloor;
    } else {
      prev->next = newFloor;
    }
    prev = newFloor;
  }
  prev->next = headFloor; // Close the circle
  log_action("System Initialized.");
}

// Queue Operations
void enqueue(Queue *q, int id) {
  Car *newCar = (Car *)malloc(sizeof(Car));
  newCar->id = id;
  newCar->entry_time = time(NULL);
  newCar->next = NULL;

  if (q->rear == NULL) {
    q->front = q->rear = newCar;
  } else {
    q->rear->next = newCar;
    q->rear = newCar;
  }
  q->count++;
}

int dequeue(Queue *q) {
  if (q->front == NULL)
    return -1;

  Car *temp = q->front;
  int id = temp->id;
  q->front = q->front->next;

  if (q->front == NULL) {
    q->rear = NULL;
  }
  q->count--;
  free(temp);
  return id;
}

// Stack Operations
void push(Stack *s, int value) {
  if (s->top < (MAX_FLOORS * SLOTS_PER_FLOOR) - 1) {
    s->items[++(s->top)] = value;
  }
}

int pop(Stack *s) {
  if (s->top >= 0) {
    return s->items[(s->top)--];
  }
  return -1;
}

// Bit Manipulation
int find_free_slot(uint64_t slots) {
  for (int i = 0; i < SLOTS_PER_FLOOR; i++) {
    if (!((slots >> i) & 1)) {
      return i;
    }
  }
  return -1; // No free slot
}

void set_slot(uint64_t *slots, int bitIndex) { *slots |= (1ULL << bitIndex); }

void clear_slot(uint64_t *slots, int bitIndex) {
  *slots &= ~(1ULL << bitIndex);
}

bool is_slot_occupied(uint64_t slots, int bitIndex) {
  return (slots >> bitIndex) & 1;
}

// Core Logic
void add_car_to_entry() {
  enqueue(&entryQueue, carIdCounter++);
  char msg[100];
  sprintf(msg, "Car #%d joined Entry Queue", carIdCounter - 1);
  log_action(msg);
}

void process_entry() {
  if (entryQueue.count == 0) {
    log_action("Entry Queue is empty!");
    return;
  }

  // Find a slot
  Floor *temp = headFloor;
  int slotIndex = -1;
  Floor *targetFloor = NULL;

  // Iterate through floors to find space
  do {
    slotIndex = find_free_slot(temp->slots);
    if (slotIndex != -1) {
      targetFloor = temp;
      break;
    }
    temp = temp->next;
  } while (temp != headFloor);

  if (targetFloor != NULL) {
    int carId = dequeue(&entryQueue);
    set_slot(&(targetFloor->slots), slotIndex);
    push(&evacuationStack, carId);

    char msg[100];
    sprintf(msg, "Car #%d Parked: Floor %d, Slot %d", carId,
            targetFloor->floor_number, slotIndex + 1);
    log_action(msg);
  } else {
    log_action("PARKING FULL! Please wait.");
  }
}

void request_exit() {
  Floor *temp = headFloor;
  bool found = false;
  do {
    if (temp->slots != 0) {
      for (int i = 0; i < 64; i++) {
        if (is_slot_occupied(temp->slots, i)) {
          enqueue(&exitQueue, 999); // Dummy ID
          clear_slot(&(temp->slots), i);
          found = true;
          char msg[100];
          sprintf(msg, "Car leaving Floor %d, Slot %d", temp->floor_number,
                  i + 1);
          log_action(msg);
          break;
        }
      }
    }
    if (found)
      break;
    temp = temp->next;
  } while (temp != headFloor);

  if (!found) {
    log_action("No cars to exit!");
  }
}

void process_exit() {
  if (exitQueue.count == 0) {
    log_action("Exit Queue is empty!");
    return;
  }

  int carId = dequeue(&exitQueue);
  log_action("Payment Processed. Car Exited.");
}

void simulate_emergency() {
  log_action("!!! EMERGENCY EVACUATION STARTED !!!");

  while (evacuationStack.top >= 0) {
    int carId = pop(&evacuationStack);
    char msg[100];
    sprintf(msg, "Evacuating Car #%d", carId);
    log_action(msg);
    // sleep(1); // Optional delay for effect
  }

  // Clear all slots
  Floor *temp = headFloor;
  do {
    temp->slots = 0;
    temp = temp->next;
  } while (temp != headFloor);

  log_action("Evacuation Complete. All slots empty.");
}

// UI Visualization Helpers

void draw_queue_horizontal(Queue *q, const char *label) {
  printf("\n%s (%d cars):\n", label, q->count);
  if (q->count == 0) {
    printf("  [ EMPTY ]\n");
    return;
  }

  Car *temp = q->front;
  // Top border
  for (int i = 0; i < q->count; i++)
    printf("+-------+ ");
  printf("\n");
  // Content
  while (temp) {
    printf("| ID:%2d | ", temp->id);
    temp = temp->next;
  }
  printf("\n");
  // Bottom border
  for (int i = 0; i < q->count; i++)
    printf("+-------+ ");
  printf("\n");
}

void draw_stack_vertical(Stack *s, const char *label) {
  printf("\n%s (Top %d shown):\n", label, (s->top + 1 > 5 ? 5 : s->top + 1));
  if (s->top == -1) {
    printf("  [ EMPTY ]\n");
    return;
  }

  // Show top 5 items
  int count = 0;
  for (int i = s->top; i >= 0 && count < 5; i--) {
    printf("  +-------+\n");
    printf("  | ID:%2d |\n", s->items[i]);
    printf("  +-------+\n");
    count++;
  }
  if (s->top >= 5)
    printf("     ...\n");
}

void print_binary_grid(uint64_t n) {
  printf("  +-------------------------------+\n");
  for (int row = 0; row < 4; row++) { // 4 rows of 16 slots = 64 slots
    printf("  | ");
    for (int col = 0; col < 16; col++) {
      int bitIndex = row * 16 + col;
      printf("%s", ((n >> bitIndex) & 1) ? "X" : ".");
      if (col < 15)
        printf(" ");
    }
    printf(" |\n");
  }
  printf("  +-------------------------------+\n");
  printf("    (X = Occupied, . = Empty)\n");
}

void display_ui() {
  // Clear screen (ANSI escape code)
  // printf("\033[H\033[J");

  printf("\n============================================================\n");
  printf("               SMART PARKING SYSTEM v2.0                    \n");
  printf("============================================================\n");

  // 1. Queues
  draw_queue_horizontal(&entryQueue, "ENTRY QUEUE");

  // 2. Building Layout
  printf("\nBUILDING STATUS:\n");
  printf("   Current View: [ LEVEL %d ]\n", currentFloorDisplay->floor_number);
  print_binary_grid(currentFloorDisplay->slots);

  // 3. Stack (Evacuation Order)
  // Show stack side-by-side with building if possible, but sequential is fine
  // for CLI
  draw_stack_vertical(&evacuationStack, "PARKING STACK (LIFO Tracking)");

  // 4. Stats
  int occupied = 0;
  for (int i = 0; i < 64; i++)
    if ((currentFloorDisplay->slots >> i) & 1)
      occupied++;
  printf("\nSTATS: Available: %d/64 | Rate: $%.2f/hr\n", 64 - occupied,
         PRICE_PER_HOUR);

  draw_queue_horizontal(&exitQueue, "EXIT QUEUE");

  // 5. Action Log
  printf("\nACTION LOG:\n");
  printf("+----------------------------------------------------------+\n");
  for (int i = 0; i < LOG_SIZE; i++) {
    if (strlen(actionLog[i]) > 0)
      printf("| %-56s |\n", actionLog[i]);
    else
      printf("| %-56s |\n", " ");
  }
  printf("+----------------------------------------------------------+\n");
}

int main() {
  init_system();
  int choice;

  while (1) {
    display_ui();
    printf("\nMENU:\n");
    printf("1. [Entry] Add Car to Queue\n");
    printf("2. [Entry] Process Entry (Park Car)\n");
    printf("3. [View]  Rotate Floor View\n");
    printf("4. [Exit]  Request Exit\n");
    printf("5. [Exit]  Process Exit Payment\n");
    printf("6. [Emerg] EMERGENCY EVACUATION\n");
    printf("7. [Sys]   Quit\n");
    printf("Select option: ");

    if (scanf("%d", &choice) != 1) {
      while (getchar() != '\n')
        ; // Clear buffer
      continue;
    }

    switch (choice) {
    case 1:
      add_car_to_entry();
      break;
    case 2:
      process_entry();
      break;
    case 3:
      currentFloorDisplay = currentFloorDisplay->next;
      break;
    case 4:
      request_exit();
      break;
    case 5:
      process_exit();
      break;
    case 6:
      simulate_emergency();
      break;
    case 7:
      printf("Exiting system...\n");
      return 0;
    case 8: // Debug: Fill current floor
      currentFloorDisplay->slots = ~0ULL;
      log_action("DEBUG: Current Floor Filled!");
      break;
    default:
      log_action("Invalid Option Selected!");
    }
  }
  return 0;
}
