/*
 *  FWPCaloHitProxyBuilder.cc
 *  FWorks
 *
 *  Created by Ianna Osborne on 9/9/10.
 *  Copyright 2010 FNAL. All rights reserved.
 *
 */

#include "Fireworks/Core/interface/FWDigitSetProxyBuilder.h"
#include "Fireworks/Core/interface/FWGeometry.h"
#include "Fireworks/Core/interface/FWEventItem.h"
#include "Fireworks/Core/interface/BuilderUtils.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "SimDataFormats/CaloHit/interface/PCaloHit.h"
#include "SimDataFormats/CaloHit/interface/PCaloHitContainer.h"

class FWPCaloHitProxyBuilder : public FWDigitSetProxyBuilder
{
public:
   FWPCaloHitProxyBuilder( void ) {} 
   virtual ~FWPCaloHitProxyBuilder( void ) {}

   REGISTER_PROXYBUILDER_METHODS();

private:
   FWPCaloHitProxyBuilder( const FWPCaloHitProxyBuilder& );
   const FWPCaloHitProxyBuilder& operator=( const FWPCaloHitProxyBuilder& );

   virtual void build( const FWEventItem* iItem, TEveElementList* product, const FWViewContext* );
};

void FWPCaloHitProxyBuilder::build(const FWEventItem* iItem, TEveElementList* product, const FWViewContext*)
{
   const edm::PCaloHitContainer *collection = 0;
   iItem->get( collection );
   if (! collection)
      return;


   TEveBoxSet* boxSet = addBoxSetToProduct(product);
   for (std::vector<PCaloHit>::const_iterator it = collection->begin() ; it != collection->end(); ++it)
   {  
      const float* corners = item()->getGeom()->getCorners((*it).id());

      std::vector<float> scaledCorners(24);
      if (corners)
         fireworks::energyTower3DCorners(corners, (*it).energy() * 10, scaledCorners);

      addBox(boxSet, &scaledCorners[0]);
   }
}

// !AMT TEveBoxSet is not projectable. Can't be added to RPZ views empty.
REGISTER_FWPROXYBUILDER( FWPCaloHitProxyBuilder, edm::PCaloHitContainer, "PCaloHits", FWViewType::kAll3DBits | FWViewType::kAllRPZBits );