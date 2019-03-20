# TTL

Collection of usefull C++ templates utils, based on Mix-in classes concepts(https://en.wikipedia.org/wiki/Mixin), which extending user-class functionality by providing additional capabilities. The following capabilities are provided at now:

1) EventFramework
2) Serialize/Deserialize framework
3) other utils: 
      thread_local variables storage per class object, not static per class (In MAP-REDUCE typical usage)
      LocklessWriter - concurrent file writer (based on std::atomic)
      
# EventFramework
  Simple template-extendable event driven framework, the main advantage of which are absent of **virtual** call and **dynamic_cast**; with flexible event configuration. 
  Let's make an event consumer and subscribe it on specific types(and values) from event storage. 
  Look at the following example:
  
  **The First Step**: declare your subscriber class
  
    //Event Subscriber class
    struct EventSubscriber :
        public IEventConsumer<EventSubscriber, MouseEvent, KeyboardEvent, TestEvent /*AnotherEvent*/>
    {
        //Specific processing event methods, based on event type
        urc::ResultDescription processSpecificEvent(const MouseEvent &event, MouseEventCMD type) { /*TODO*/ }
        urc::ResultDescription processSpecificEvent(const KeyboardEvent &event, KeyboardEventCMD type) { /*TODO*/ }
        urc::ResultDescription processSpecificEvent(const TestEvent &event, CustomEventCMD type) { /*TODO*/ }
    };
  
   `IEventConsumer` is a static interface (Mix-In) here, which force your derived class to implement specific methods. Because your register the following event types: `MouseEvent, KeyboardEvent, TestEvent` you need to define specific methods for these vent types `processSpecificEvent()`
  You can register all event types in 'global' framework:
  
    using EventFramework = EventFrameworkFactory<MouseEvent, KeyboardEvent, TestEvent>;
    
   It can helps you to collect all types in the one place and provides some usefull helper functions - `<createControllerEvent>` for example.
   
   **The Second Step**: configure event subscriptions
   
   Initialize your Event subscriber instance for some events usage from configuration (use std::map for example, but flexible intrface provided too):
   
    Configurator conf;
    conf.configurationMap.insert({"ControllableEvents", "KEYBOARD_EVENT,MOUSE_EVENT,TestEvent"});  /*event types to subscription*/
    conf.configurationMap.insert({"KEYBOARD_EVENT", "MOVE_FORWARD,MOVE_BACKWARD"});                /*specific events in event type*/
    conf.configurationMap.insert({"MOVE_FORWARD", "w"});                                         /*event and its command*/
    conf.configurationMap.insert({"MOVE_BACKWARD", "s"});
    conf.configurationMap.insert({"MOUSE_EVENT", "LOOK"});                                         /*another event type*/
    conf.configurationMap.insert({"LOOK", "MOUSE_MOVE"});
    conf.configurationMap.insert({"TestEvent", "TEID_1_CMD"});                                     /*another event type*/
    conf.configurationMap.insert({"TEID_1_CMD", "TEID_1"});
   
   Configure consumer for event subsription
   
    EventSubscriber consumer;
    consumer.loadControlEvents(conf);
    
   **The Final Step**: Usage
   
    /*Somewhere in code: generate event*/
    auto event = EventFramework::createControllerEvent<MouseEvent>(
                                0.0f, 0.0f, //{coordnates X,Y}
                                MouseButton::MOUSE_MOVE,
                                0,  //key modifier
                                MouseButtonState::MB_STATE_NONE);
    eventProcessor.push_back(event);
    
    /*Somewhere in processing loop...*/
    for(auto &event: receivedEvents)
    {
      consumer.onProcessEventDispatcher(event);
    }
    
    /*Appropriate method would be called*/
    urc::ResultDescription processSpecificEvent(const MouseEvent &event, MouseEventCMD type)
    {
      //Gotcha!
    }
    
# Serialize/Deseriaize
  Let's imagine, that you have several classes for which you need provide serialize/deserialize capability (store data in files and load data from files, for example). In common way you need to implement abstract class which provide serialize/deserialize interface, for example:
  
    struct ISerializable
    {
        virtual ~ISerialize() = default;
        virtual bool serialize() = 0;
        virtual bool deserialize() = 0;
     };
  
  So, this interface required to be implemented in all your classes: every class will implement their own serialize/deserialize capability. Every software developer will provide own implementation of this interface with own types and logics...
  
  Look at the typical TTL implementation usage example:
  
    struct A : public ISerializable<A>
    {
      enum
      {
          isSerializableSupport = true
      };

      //Serializable data
      std::string txt;
      int num;
      std::vector<double> doubleVector;

      //Impl
      bool serializeImpl(std::ostream &out)
      {
        serializeParams(out, txt, num, doubleVector);
        return true;
      }
      bool deserializeImpl(std::istream &out)
      {
          deserializeParams(out, txt, num, doubleVector);
          return true;
      }
    };

    struct B : public ISerializable<B>
    {
      enum
      {
          isSerializableSupport = false
      };

      //No Impl, default stub behavior
    };
    
    //USAGE: Somewhere in code...
    A a;
    ...
    std::stringstream ss;
    a.serialize(ss);
    ...
    A aDup;
    aDup.deserialize(ss);
It's looked: Short, pretty, extendable!

