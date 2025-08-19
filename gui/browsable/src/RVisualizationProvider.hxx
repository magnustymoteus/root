//
// Created by patryk on 15.08.25.
//

#ifndef ROOT_RVISUALIZATIONPROVIDER_HXX
#define ROOT_RVISUALIZATIONPROVIDER_HXX

#include <ROOT/Browsable/RProvider.hxx>

#include "RVisualizationHolder.hxx"
#include "RTreeMapPainter.hxx"

/** \class RVisualizationProvider
\ingroup rbrowser
\brief Provider for RNTuple TreeMap visualization on TCanvas
\author Patryk Pilichowski
\date 2025
\warning This is part of the ROOT 7 prototype! It will change without notice. It might trigger earthquakes. Feedback is welcome!
*/
class RVisualizationProvider : public RProvider {
public:
   /** Create TreeMap visualization for RNTuple */
   std::unique_ptr<ROOT::Experimental::RTreeMapPainter> CreateTreeMap(RVisualizationHolder *holder) const
   {
      if (!holder)
         return nullptr;

      return ROOT::Experimental::RTreeMapPainter::Import(
            holder->GetFileName(), holder->GetTupleName());
   }
};

#endif // ROOT_RVISUALIZATIONPROVIDER_HXX
