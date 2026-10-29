#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DATA_FILE "complaints.txt"
#define TEXT_SIZE 60
typedef struct Complaint 
{
int id;
char studentName[TEXT_SIZE];
char roomNo[TEXT_SIZE];
char category[TEXT_SIZE];
int priority;                
char status[TEXT_SIZE];
struct Complaint *next;
} Complaint;
Complaint *head = NULL;
int nextId = 1001;
void readLine(const char *prompt, char *buffer, int size) {
printf("%s", prompt);
if (fgets(buffer, size, stdin) != NULL)
{
buffer[strcspn(buffer, "\n")] = '\0';
}
else
{
buffer[0] = '\0';
}
}
int readInt(const char *prompt, int min, int max) 
{
char line[100];
char *end;
long value;
while (1) 
{
readLine(prompt, line, sizeof(line));
value = strtol(line, &end, 10);
if (end != line && *end == '\0' && value >= min && value <= max) 
{
return (int)value;
}
printf("Please enter a number from %d to %d.\n", min, max);
}
}
void insertByPriority(Complaint *newComplaint)
{
Complaint *current;
if (head == NULL || newComplaint->priority > head->priority) {
newComplaint->next = head;
head = newComplaint;
return;
}
current = head;
while (current->next != NULL && current->next->priority >= newComplaint->priority)
{
current = current->next;
}
newComplaint->next = current->next;
current->next = newComplaint;
}
void saveToFile(void) {
FILE *file = fopen(DATA_FILE, "w");
Complaint *current = head;
if (file == NULL) 
{
printf("Warning: Could not save complaint records.\n");
return;
}
while (current != NULL) 
{
fprintf(file, "%d|%s|%s|%s|%d|%s\n",
current->id, current->studentName, current->roomNo,
current->category, current->priority, current->status);
current = current->next;
}
fclose(file);
}
void loadFromFile(void) 
{
FILE *file = fopen(DATA_FILE, "r");
char line[400];
if (file == NULL) return;
while (fgets(line, sizeof(line), file) != NULL)
{
Complaint *item = (Complaint *)malloc(sizeof(Complaint));
char *token;
if (item == NULL) break;
line[strcspn(line, "\n")] = '\0';
token = strtok(line, "|");
if (token == NULL) 
{
free(item); continue;
}
item->id = atoi(token);
token = strtok(NULL, "|"); 
if (!token) 
{ 
free(item); 
continue; 
}
snprintf(item->studentName, TEXT_SIZE, "%s", token);
token = strtok(NULL, "|"); 
if (!token) 
{ 
free(item); 
continue; 
}
snprintf(item->roomNo, TEXT_SIZE, "%s", token);
token = strtok(NULL, "|"); 
if (!token) 
{ 
free(item); 
continue; 
}
snprintf(item->category, TEXT_SIZE, "%s", token);
token = strtok(NULL, "|"); 
if (!token) 
{ 
free(item); 
continue; 
}
item->priority = atoi(token);
token = strtok(NULL, "|"); 
if (!token) 
{ 
free(item); 
continue; 
}
snprintf(item->status, TEXT_SIZE, "%s", token);
item->next = NULL;
insertByPriority(item);
if (item->id >= nextId) nextId = item->id + 1;
}
fclose(file);
}
void registerComplaint(void) 
{
Complaint *item = (Complaint *)malloc(sizeof(Complaint));
int choice;
if (item == NULL) 
{
printf("Memory allocation failed.\n");
return;
}
item->id = nextId++;
readLine("Enter student name: ", item->studentName, TEXT_SIZE);
readLine("Enter room number: ", item->roomNo, TEXT_SIZE);
readLine("Enter complaint category (Water/Electrical/Fan/Plumbing/Other): ", item->category, TEXT_SIZE);
printf("Priority levels: 1 = Low, 2 = Medium, 3 = High\n");
choice = readInt("Enter priority (1-3): ", 1, 3);
item->priority = choice;
snprintf(item->status, TEXT_SIZE, "Pending");
item->next = NULL;
insertByPriority(item);
saveToFile();
printf("\nComplaint registered successfully!\n");
printf("Your Complaint ID is: %d\n", item->id);
}
void displayComplaints(void) 
{
Complaint *current = head;
if (current == NULL) 
{
printf("\nNo complaints registered yet.\n");
return;
}
printf("\n----- HOSTEL COMPLAINT LIST (Priority Order) -----\n");
printf("ID\tStudent\t\tRoom\tCategory\tPriority\tStatus\n");
printf("------------------------------------------------------------\n");
while (current != NULL) 
{
const char *priorityText = current->priority == 3 ? "High" :
current->priority == 2 ? "Medium" : "Low";
printf("%d\t%-15s\t%-6s\t%-12s\t%-8s\t%s\n",
current->id, current->studentName, current->roomNo,
current->category, priorityText, current->status);
current = current->next;
}
}
void searchComplaint(void) 
{
int id = readInt("Enter Complaint ID to search: ", 1, 1000000000);
Complaint *current = head;
while (current != NULL) 
{
if (current->id == id) 
{
printf("\nComplaint found!\n");
printf("Complaint ID : %d\n", current->id);
printf("Student Name : %s\n", current->studentName);
printf("Room Number  : %s\n", current->roomNo);
printf("Category     : %s\n", current->category);
printf("Priority     : %s\n", current->priority == 3 ? "High" : current->priority == 2 ? "Medium" : "Low");
printf("Status       : %s\n", current->status);
return;
}
current = current->next;
}
printf("No complaint found with ID %d.\n", id);
}
void updateStatus(void)
{
int id = readInt("Enter Complaint ID to update: ", 1, 1000000000);
Complaint *current = head;
while (current != NULL) 
{
if (current->id == id) 
{
printf("1. Pending\n2. In Progress\n3. Resolved\n");
switch (readInt("Choose new status (1-3): ", 1, 3)) 
{
case 1: snprintf(current->status, TEXT_SIZE, "Pending"); break;
case 2: snprintf(current->status, TEXT_SIZE, "In Progress"); break;
case 3: snprintf(current->status, TEXT_SIZE, "Resolved"); break;
}
saveToFile();
printf("Complaint status updated successfully.\n");
return;
}
current = current->next;
}
printf("No complaint found with ID %d.\n", id);
}
void freeList(void) 
{
Complaint *current = head;
while (current != NULL) 
{
Complaint *temp = current;
current = current->next;
free(temp);
}
}
int main(void) 
{
int choice;
loadFromFile();
printf(" -----HOSTEL COMPLAINT PRIORITY MANAGEMENT SYSTEM-----\n");
do 
{
printf("\n1. Register Complaint\n");
printf("2. Display All Complaints\n");
printf("3. Search Complaint by ID\n");
printf("4. Update Complaint Status\n");
printf("5. Exit\n");
choice = readInt("Enter your choice (1-5): ", 1, 5);
switch (choice) 
{
case 1: registerComplaint();  
break;
case 2: displayComplaints(); 
break;
case 3: searchComplaint();
break;
case 4: updateStatus();
break;
case 5: printf("Thank you for using the system!\n"); 
break;
}
} 
while (choice != 5);
freeList();
return 0;
}
