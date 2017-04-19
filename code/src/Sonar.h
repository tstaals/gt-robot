
class Sonar {
  public:
    Sonar(int trigger, int echo);
    void ping();
    bool isFinished(){ return _finished; }
    void _echo_isr();
    unsigned int getRange();

  private:
    int _trigger, _echo;
    volatile unsigned long _start, _end;
    volatile bool _finished;
};
