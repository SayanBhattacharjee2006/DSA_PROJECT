#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <limits>
using namespace std;

class Group;
class Node;

class Contact{
private:
    string name;
    string number;
    string email;
    string address;
    //can not change the name and number as they are used in contactMap and phoneMap as a key for indexing
public:
    Contact(string name, string number, string email = "", string address = ""):name(name),number(number),email(email),address(address){}

    // getter functions
    string getName() { return this->name; }
    string getNumber() { return this->number; }
    string getEmail() { return this->email; }
    string getAddress() { return this->address; }

    // setter functions
    void setEmail(string email){
        this->email = email;
    }

    void setAddress(string address){
        this->address = address;
    }

    // display the contact details
    void display(){
        cout << "-----Contact Details-----" << endl;
        cout << "Name : " << this->name << endl;
        cout << "Number : " << this->number << endl;
        cout << "Email : " << this->email << endl;
        cout << "Address : " << this->address << endl;
    }
};

class Node{
    public: 
    
        Contact val;
        Node *next;
        Node *prev;
        unordered_set<Group*> groups;

        Node(const Contact &val):val(val),next(nullptr),prev(nullptr){}
};


class Group{
    private:
        string groupName;
        string groupDescription;
        unordered_set<Node*>groupMembers;

    public:
        Group(string name, string description=""):groupName(name),groupDescription(description){}

    string getGroupName(){
        return this->groupName;
    }

    string getGroupDescription(){
        return this->groupDescription;
    }

    bool addMember(Node* newMember){
        if(groupMembers.find(newMember) != groupMembers.end()){
            return false; // Member already exists in the group
        }

        groupMembers.insert(newMember);
        return true;
    }

    bool removeMember(Node* member){
        if(groupMembers.find(member)== groupMembers.end()){
            return false; // member does not exist in the group
        }

        groupMembers.erase(member);
        return true;
    }

    void setDescription(string description){
        this->groupDescription = description;
    }

    void displayGroupDetails(){
        cout<<"-----Group Details-----"<<endl;
        cout<<"Group Name : "<<this->groupName<<endl;
        cout<<"Group Description : "<<this->groupDescription<<endl;
        cout<<"Group Members Count : "<<this->groupMembers.size()<<endl;
    }

    int getMembersCount(){
        return this->groupMembers.size();
    }

    unordered_set<Node*>& getGroupMembers(){
        return this->groupMembers;
    }

    bool isMember(Node* member){
        return groupMembers.find(member) != groupMembers.end();
    }
       
};


class ContactManager{
    private:
        Node *head;
        Node *tail;
        int contactsCount;

        unordered_map<string, Node *> contactMap; //used to search contact by name
        unordered_map<string, Node *> phoneMap; // used to search contact by number
        unordered_map<string, Group*> groupMap; // used to search group by name

    public:

        ContactManager(){
            head = nullptr;
            tail = nullptr;
            contactsCount = 0;
        }

        int getContactsCount(){
            return this->contactsCount;
        }

        void addContactInList(Contact contact){
            Node* newNode = new Node(contact);
            if(!head){
                head = newNode;
                tail = newNode;
            }else{
                tail -> next = newNode;
                newNode -> prev = tail;
                tail = newNode;
            }
            return ;

        }

        void addContact(string name, string number,string email = "",string address=""){
            if(name.empty() || number.empty()){
                cout<<"ADD CONTACT FAILED: Name and Number are required fields."<<endl;
                return;
            }

            if(contactMap.find(name)!=contactMap.end()){
                cout<<"ADD CONTACT FAILED: Contact with name '"<<name<<"' already exists."<<endl;
                return;
            }

            if(phoneMap.find(number)!=phoneMap.end()){
                cout<<"ADD CONTACT FAILED: Contact with number '"<<number<<"' already exists."<<endl;
                return;
            }

            Contact newContact(name,number,email,address);
            addContactInList(newContact);
            contactsCount++;
            contactMap[name] = tail;
            phoneMap[number] = tail;
            cout<<"CONTACT ADDED SUCCESSFULLY: "<<name<<endl;
            
        }

        void updateContactByName(string name, string updatedEmail="", string updatedAddress=""){
            if(contactMap.find(name)==contactMap.end()){
                cout<<"UPDATE CONTACT FAILED: Contact with name '"<<name<<"' does not exist."<<endl; 
                return;
            }

            if(updatedEmail.empty() && updatedAddress.empty()){
                cout<<"UPDATE CONTACT FAILED: At least one field (email or address) must be provided for update."<<endl;
                return;
            }

            Node* contactNodeToUpdate = contactMap[name];
            if(!updatedEmail.empty()){
                contactNodeToUpdate->val.setEmail(updatedEmail);
            }
            if(!updatedAddress.empty()){
                contactNodeToUpdate->val.setAddress(updatedAddress);
            }
            cout<<"CONTACT UPDATED SUCCESSFULLY: "<<name<<endl;
        }

        void updateContactByNumber(string number, string updatedEmail = "", string updatedAddress=""){
            if(phoneMap.find(number) == phoneMap.end()){
                cout<<"UPDATE CONTACT FAILED: Contact with number '"<<number<<"' does not exist."<<endl; 
                return;
            }

            if(updatedAddress.empty() && updatedEmail.empty()){
                cout<<"UPDATE CONTACT FAILED: At least one field (email or address) must be provided for update."<<endl;
                return;
            }

            Node* contactNodeToUpdate = phoneMap[number];
            if(!updatedEmail.empty()){
                contactNodeToUpdate->val.setEmail(updatedEmail);
            }
            if(!updatedAddress.empty()){
                contactNodeToUpdate->val.setAddress(updatedAddress);
            }
            cout<<"CONTACT UPDATED SUCCESSFULLY: "<<contactNodeToUpdate->val.getName()<<endl;
        }

        void deleteContactByName(string name){
            if(contactMap.find(name)==contactMap.end()){
                cout<<"DELETE CONTACT FAILED: Contact with name '"<<name<<"' does not exist."<<endl; 
                return;
            }

            Node* contactToDelete = contactMap[name];

            string number = contactToDelete->val.getNumber();

            if(contactToDelete->prev){
                contactToDelete->prev->next = contactToDelete->next;
            }else{
                head = contactToDelete->next;
            }

            if(contactToDelete->next){
                contactToDelete->next->prev = contactToDelete->prev;
            }else{
                tail = contactToDelete->prev;
            }

            for(Group* group : contactToDelete->groups){
                group->removeMember(contactToDelete);
            }

            contactMap.erase(name);
            phoneMap.erase(number);
            contactsCount--;
            delete contactToDelete;
            cout<<"CONTACT DELETED SUCCESSFULLY: "<<name<<endl;
        }
        
        void deleteContactByNumber(string number){
            if(phoneMap.find(number) == phoneMap.end()){
                cout<<"DELETE CONTACT FAILED: Contact with number '"<<number<<"' does not exist."<<endl; 
                return;
            }
            
            Node* contactToDelete = phoneMap[number];
            
            string name = contactToDelete->val.getName();
            
            if(contactToDelete->prev){
                contactToDelete->prev->next = contactToDelete->next;
            }else{
                head = contactToDelete->next;
            }
            
            if(contactToDelete->next){
                contactToDelete->next->prev = contactToDelete->prev;
            }else{
                tail = contactToDelete->prev;
            }

            for(Group* group : contactToDelete->groups){
                group->removeMember(contactToDelete);
            }

            contactMap.erase(name);
            phoneMap.erase(number);
            contactsCount--;
            delete contactToDelete;
            cout<<"CONTACT DELETED SUCCESSFULLY: "<<name<<endl;
        
        }

        void displayAllContact(int order){//order = 1 for newest last and -1 for newest first
            if(!head){
                cout<<"No contacts to display."<<endl;
                return;     
            }
            if(order!=1 && order!=-1){
                cout<<"INVALID ORDER PARAMETER: Use 1 for newest last and -1 for newest first."<<endl;
                return;
            }
            Node* temp = order==-1?tail:head;
           
            while(temp){
                cout<<"-------------------------"<<endl;
                temp->val.display();
                temp = order==-1?temp->prev:temp->next;
                cout<<"\n";
            }
        }

        void displayContactDetailsByName(string name){
        
            if(contactMap.find(name)==contactMap.end()){
                cout<<"Contact with name '"<<name<<"' does not exist."<<endl; 
                return;
            }

            contactMap[name]->val.display();
        }

        void displayContactDetailsByNumber(string number){
            if(phoneMap.find(number) == phoneMap.end()){
                cout<<"Contact with number '"<<number<<"' does not exist."<<endl; 
                return;
            }

            phoneMap[number]->val.display();
        }
    
        void createGroup(string groupName,string groupDescription=""){
            if(groupName.empty()){
                cout<<"CREATE GROUP FAILED: Group name is required."<<endl;
                return;
            }

            if(groupMap.find(groupName) != groupMap.end()){
                cout<<"CREATE GROUP FAILED: Group with name '"<<groupName<<"' already exists."<<endl;
                return;
            }

            Group* newGroup = new Group(groupName,groupDescription);
            groupMap[groupName] = newGroup;
            cout<<"GROUP CREATED SUCCESSFULLY: "<<groupName<<endl;
            return ;
        }

        void deleteGroup(string groupName){
            if(groupMap.find(groupName) == groupMap.end()){
                cout<<"DELETE GROUP FAILED: Group with name '"<<groupName<<"' does not exist."<<endl;
                return;
            }

            Group* groupToDelete = groupMap[groupName];
            for(Node* member : groupToDelete->getGroupMembers()){
                member->groups.erase(groupToDelete);
            }
            groupMap.erase(groupName);
            delete groupToDelete;
            cout<<"GROUP DELETED SUCCESSFULLY: "<<groupName<<endl;
        }

        void addContactToGroup(string name, string groupName){
            if(groupMap.find(groupName) == groupMap.end()){
                cout<<"ADD CONTACT TO GROUP FAILED: Group with name '"<<groupName<<"' does not exist."<<endl;
                return;
            }

            if(contactMap.find(name) == contactMap.end()){
                cout<<"ADD CONTACT TO GROUP FAILED: Contact with name '"<<name<<"' does not exist."<<endl;
                return;
            }

            Node* contactToAdd = contactMap[name];
            Group* groupToAdd = groupMap[groupName];
            if(groupToAdd->isMember(contactToAdd)){
                cout<<"ADD CONTACT TO GROUP FAILED: Contact '"<<name<<"' is already a member of group '"<<groupName<<"'."<<endl;
                return;
            }

            groupToAdd->addMember(contactToAdd);
            contactToAdd->groups.insert(groupToAdd);
            cout<<"CONTACT '"<<name<<"' ADDED TO GROUP '"<<groupName<<"' SUCCESSFULLY."<<endl;
        }

        void removeContactFromGroup(string name, string groupName){
            if(groupMap.find(groupName) == groupMap.end()){
                cout<<"REMOVE CONTACT FROM GROUP FAILED: Group with name '"<<groupName<<"' does not exist."<<endl;
                return;
            }

            if(contactMap.find(name) == contactMap.end()){
                cout<<"REMOVE CONTACT FROM GROUP FAILED: Contact with name '"<<name<<"' does not exist."<<endl;
                return;
            }

            Node* contactToRemove = contactMap[name];
            Group* groupToRemove = groupMap[groupName];
            if(!groupToRemove->isMember(contactToRemove)){
                cout<<"REMOVE CONTACT FROM GROUP FAILED: Contact '"<<name<<"' is not a member of group '"<<groupName<<"'."<<endl;
                return;
            }

            groupToRemove->removeMember(contactToRemove);
            contactToRemove->groups.erase(groupToRemove);
            cout<<"CONTACT '"<<name<<"' REMOVED FROM GROUP '"<<groupName<<"' SUCCESSFULLY."<<endl;
        }

        void displayGroupDetails(string groupName){
            if(groupMap.find(groupName) == groupMap.end()){
                cout<<"DISPLAY GROUP DETAILS FAILED: Group with name '"<<groupName<<"' does not exist."<<endl;
                return;
            }

            groupMap[groupName]->displayGroupDetails();
        }
    
        void displayGroupMembers(string groupName){
            if(groupMap.find(groupName) == groupMap.end()){
                cout<<"DISPLAY GROUP MEMBERS FAILED: Group with name '"<<groupName<<"' does not exist."<<endl;
                return;
            }

            Group* group = groupMap[groupName];
            if(group->getMembersCount() == 0){
                cout<<"No members in group '"<<groupName<<"' to display."<<endl;
                return;
            }

            cout<<"-----Members of Group '"<<groupName<<"'-----"<<endl;
            for(Node* member : group->getGroupMembers()){
                member->val.display();
            }
        }
};


class currentLoggedInUser{
    string userName, userNumber, userEmail, userAddress;
    ContactManager contactManager;
public:
    currentLoggedInUser(string name, string number, string email="", string address=""):userName(name), userNumber(number), userEmail(email), userAddress(address){}

    string getUserName(){
        return this->userName;
    }

    string getUserNumber(){
        return this->userNumber;
    }

    string getUserEmail(){
        return this->userEmail;
    }

    string getUserAddress(){
        return this->userAddress;
    }

    void setUserName(string name){
        this->userName = name;
    }

    void setUserNumber(string number){
        this->userNumber = number;
    }

    void setUserEmail(string email){
        this->userEmail = email;
    }

    void setUserAddress(string address){
        this->userAddress = address;
    }

    void addContact(string name, string number, string email="", string address=""){
        contactManager.addContact(name, number, email, address);
    }

    void updateContactByName(string name, string updatedEmail="", string updatedAddress=""){
        contactManager.updateContactByName(name, updatedEmail, updatedAddress);
    }

    void updateContactByNumber(string number, string updatedEmail="", string updatedAddress=""){
        contactManager.updateContactByNumber(number, updatedEmail, updatedAddress);
    }

    void deleteContactByName(string name){
        contactManager.deleteContactByName(name);
    }

    void deleteContactByNumber(string number){
        contactManager.deleteContactByNumber(number);
    }

    void displayAllContacts(int order){
        contactManager.displayAllContact(order);
    }

    void displayContactDetailsByName(string name){
        contactManager.displayContactDetailsByName(name);
    }

    void displayContactDetailsByNumber(string number){
        contactManager.displayContactDetailsByNumber(number);
    }

    void createGroup(string groupName, string groupDescription=""){
        contactManager.createGroup(groupName, groupDescription);
    }

    void deleteGroup(string groupName){
        contactManager.deleteGroup(groupName);
    }

    void addContactToGroup(string name, string groupName){
        contactManager.addContactToGroup(name, groupName);
    }

    void removeContactFromGroup(string name, string groupName){
        contactManager.removeContactFromGroup(name, groupName);
    }

    void displayGroupDetails(string groupName){
        contactManager.displayGroupDetails(groupName);
    }

    void displayGroupMembers(string groupName){
        contactManager.displayGroupMembers(groupName);
    }

    void getProfileDetails(){
        cout<<"-----User Profile Details-----"<<endl;
        cout<<"Name : "<<this->userName<<endl;
        cout<<"Number : "<<this->userNumber<<endl;
        cout<<"Email : "<<this->userEmail<<endl;
        cout<<"Address : "<<this->userAddress<<endl;
        cout<<"Total Contacts : "<<contactManager.getContactsCount()<<endl;
    }

};


int main(){
    cout<<"                     -----PHONE DICTIONARY APPLICATION-----"<<endl;
    string userName, userNumber, userEmail, userAddress;

    cout<<"Please enter your profile details to continue:"<<endl;
    cout<<"Name: ";
    getline(cin, userName);
    cout<<"Number: ";
    getline(cin, userNumber);
    cout<<"Email (optional): ";
    getline(cin, userEmail);
    cout<<"Address (optional): ";
    getline(cin, userAddress);

    currentLoggedInUser user(userName, userNumber, userEmail, userAddress);

    cout<<"\n         Welcome, "<<user.getUserName()<<"! Your phone dictionary is ready to use."<<endl;
    cout<<"   \nPlease select an option from the menu below to continue."<<endl;

    while(true){
        cout<<"\n-----MAIN MENU-----"<<endl;
        cout<<"1. Add Contact"<<endl;
        cout<<"2. Update Contact"<<endl;
        cout<<"3. Delete Contact"<<endl;
        cout<<"4. Create Group"<<endl;
        cout<<"5. Delete Group"<<endl;
        cout<<"6. Add Contact to Group"<<endl;
        cout<<"7. Remove Contact from Group"<<endl;
        cout<<"8. Display Contact Details"<<endl;
        cout<<"9. Display All Contacts"<<endl;
        cout<<"10. Display Group Details"<<endl;
        cout<<"11. Display Group Members"<<endl;
        cout<<"12. Display Profile Details"<<endl;
        cout<<"13. Exit"<<endl;

        int choice;
        cout<<"Enter your choice: ";
        cin>>choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 🔥 FIX

        switch(choice){
            case 1: {
                string name, number, email, address;
                cout<<"Enter contact details:"<<endl;
                cout<<"Name: ";
                getline(cin, name);
                cout<<"Number: ";
                getline(cin, number);
                cout<<"Email (optional): ";
                getline(cin, email);
                cout<<"Address (optional): ";
                getline(cin, address);
                user.addContact(name, number, email, address);
                break;
            }

            case 2: {
                int updateChoice;
                cout<<"Update by: 1. Name 2. Number"<<endl;
                cin>>updateChoice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 🔥 FIX

                if(updateChoice == 1){
                    string name, email, address;
                    cout<<"Enter contact name to update: ";
                    getline(cin, name);
                    cout<<"Enter new email (leave blank to skip): ";
                    getline(cin, email);
                    cout<<"Enter new address (leave blank to skip): ";
                    getline(cin, address);
                    user.updateContactByName(name, email, address);
                }else if(updateChoice == 2){
                    string number, email, address;
                    cout<<"Enter contact number to update: ";
                    getline(cin, number);
                    cout<<"Enter new email (leave blank to skip): ";
                    getline(cin, email);
                    cout<<"Enter new address (leave blank to skip): ";
                    getline(cin, address);
                    user.updateContactByNumber(number, email, address);
                }else{
                    cout<<"Invalid update choice."<<endl;
                }
                break;
            }

            case 3: {
                int deleteChoice;
                cout<<"Delete by: 1. Name 2. Number"<<endl;
                cin>>deleteChoice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 🔥 FIX

                if(deleteChoice == 1){
                    string name;
                    cout<<"Enter contact name to delete: ";
                    getline(cin, name);
                    user.deleteContactByName(name);
                }else if(deleteChoice == 2){
                    string number;
                    cout<<"Enter contact number to delete: ";
                    getline(cin, number);
                    user.deleteContactByNumber(number);
                }else{
                    cout<<"Invalid delete choice."<<endl;
                }
                break;
            }

            case 4: {
                string groupName, groupDescription;
                cout<<"Enter group name: ";
                getline(cin, groupName);
                cout<<"Enter group description (optional): ";
                getline(cin, groupDescription);
                user.createGroup(groupName, groupDescription);
                break;
            }

            case 5: {
                string groupName;
                cout<<"Enter group name to delete: ";
                getline(cin, groupName);
                user.deleteGroup(groupName);
                break;
            }

            case 6: {
                string name, groupName;
                cout<<"Enter contact name to add to group: ";
                getline(cin, name);
                cout<<"Enter group name: ";
                getline(cin, groupName);
                user.addContactToGroup(name, groupName);
                break;
            }

            case 7: {
                string name, groupName;
                cout<<"Enter contact name to remove from group: ";
                getline(cin, name);
                cout<<"Enter group name: ";
                getline(cin, groupName);
                user.removeContactFromGroup(name, groupName);
                break;
            }

            case 8: {
                int displayChoice;
                cout<<"Display by: 1. Name 2. Number"<<endl;
                cin>>displayChoice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 🔥 FIX

                if(displayChoice == 1){
                    string name;
                    cout<<"Enter contact name to display details: ";
                    getline(cin, name);
                    user.displayContactDetailsByName(name);
                }else if(displayChoice == 2){
                    string number;
                    cout<<"Enter contact number to display details: ";
                    getline(cin, number);
                    user.displayContactDetailsByNumber(number);
                }else{
                    cout<<"Invalid display choice."<<endl;
                }
                break;
            }

            case 9: {
                int orderChoice;
                cout<<"Display order: 1. Newest Last 2. Newest First"<<endl;
                cin>>orderChoice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 🔥 FIX

                cout<<"\nDisplaying all contacts:"<<endl;
                if(orderChoice == 1){
                    user.displayAllContacts(1);
                }else if(orderChoice == 2){
                    user.displayAllContacts(-1);
                }else{
                    cout<<"Invalid display order choice."<<endl;
                }
                break;
            }

            case 10: {
                string groupName;
                cout<<"Enter group name to display details: ";
                getline(cin, groupName);
                user.displayGroupDetails(groupName);
                break;
            }

            case 11: {
                string groupName;
                cout<<"Enter group name to display members: ";
                getline(cin, groupName);
                user.displayGroupMembers(groupName);
                break;
            }

            case 12: {
                user.getProfileDetails();
                break;
            }

            case 13: {
                cout<<"Goodbye!"<<endl;
                return 0;
            }

            default: {
                cout<<"Invalid choice. Please try again."<<endl;
            }
        }
    }

    return 0;
}