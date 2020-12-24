#include<string.h>
#include<omnetpp.h>

using namespace omnetpp;

class Txc4 : public cSimpleModule{
    private:
        int count;
        cMessage *tictocMessage;
        cMessage *event;
    protected:
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg);
        virtual void generateNewMessage();
        virtual void sendCopyOf(cMessage *msg);
    public :
        Txc4();
        virtual ~Txc4();
};

Define_Module(Txc4);
// Constructor
Txc4::Txc4(){
    event = tictocMessage = NULL;
}
// Destructor
Txc4::~Txc4(){
    cancelAndDelete(event);
    delete tictocMessage;
}

cMessage *Txc4::generateNewMessage()
{
    // Generate a message with a different name every time.
    char msgname[20];
    sprintf(msgname, "tic-%d", ++seq);
    cMessage *msg = new cMessage(msgname);
    return msg;
}

void Txc4::sendCopyOf(cMessage *msg)
{
    // Duplicate message and send the copy.
    cMessage *copy = (cMessage *) msg->dup();
    send(copy, "out");
}

void Txc4::initialize(){
    event = new cMessage("event");
    tictocMessage = NULL;
    simtime_t delay = par("delayTime");

    count = par("limit");
    WATCH(count);
    if(par("sendMessage").boolValue()){
        cMessage *msg = new cMessage("init network!");
        tictocMessage = new cMessage("tictocMessage");
        //send(msg, "out");
        scheduleAt(simTime()+delay, event);
    }
}

void Txc4::handleMessage(cMessage *msg){
    count--;
    if(count == 0){
        EV <<"ket thuc rooi!";
        delete msg;
    } else if(msg==event){
        EV << "event comback....";
        send(tictocMessage,"out");
    } else {
        if(strcmp("tic",getName()) == 0)
            EV << "Hello_server";
        else if(strcmp("toc",getName()) == 0)
            EV << "Hello_tic";
        else EV << "Hello_toc";
        send(msg, "out");
    }
}
