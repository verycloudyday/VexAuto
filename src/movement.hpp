#ifndef MOVEMENT_H
#define MOVEMENT_H

extern int turninverse;
extern int JB;
extern int PB;
extern int PX;
extern int JX;

struct ChassisDataSet{
  int Left;
  int Right;
  double Avg;   // Average between left and right of the drive train
  int Diff;     // Left - Right
  double HDG;   // Robot heading
};

struct PIDDataSet{
  double kp;
  double ki;
  double kd;
};

extern void Zeroing(bool dist, bool HDG);
extern ChassisDataSet ChassisUpdate();
extern void Move(int left, int right);
extern void BStop();
extern void CStop();
extern void RunRoller(int val);
extern int PrevE;
extern void MoveEncoderPID(PIDDataSet KVals, int Speed, double dist,double AccT, double ABSHDG,bool brake);
extern void MovePID(PIDDataSet KVals, PIDDataSet Dvals,  int Speed, double dist,double AccT, double ABSHDG,bool brake, double time = 10000);
extern void TurnMaxTimePID(PIDDataSet KVals,double DeltaAngle,double TE, bool brake);
extern void MoveTimePID(PIDDataSet KVals, int Speed, double TE,double AccT,double ABSHDG, bool brake);
extern void MoveTime(int Speed, double TE,double AccT, bool brake);


#endif