#ifndef SCENARIO_H
#define SCENARIO_H

#include "femobject.h"


#include "application/gmlibwrapper.h"

// qt
#include <QObject>



class Scenario : public GMlibWrapper {
  Q_OBJECT
public:
  using GMlibWrapper::GMlibWrapper;

  void    initializeScenario() override;
  void    cleanupScenario() override;
private:
 // std::vector< FEM::FEMObject*> femobject;

 };

#endif // SCENARIO_H
