/*
 Copyright (C) 2010-2012 Kristian Duske

 This file is part of TrenchBroom.

 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with TrenchBroom.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Picker.h"
#include "Model/MapObject.h"
#include "Model/Octree.h"

#include <algorithm>

namespace TrenchBroom {
    namespace Model {
        Hit::Hit(void* object, Type type, const Vec3f& hitPoint, float distance) :
        m_object(object), 
        m_index(-1),
        m_type(type),
        m_hitPoint(hitPoint),
        m_distance(distance) {}

        Hit::Hit(void* object, int index, Type type, const Vec3f& hitPoint, float distance) :
        m_object(object),
        m_index(index),
        m_type(type),
        m_hitPoint(hitPoint),
        m_distance(distance) {}

        void PickResult::sortHits() {
            sort(m_hits.begin(), m_hits.end(), CompareHitsByDistance());
            m_sorted = true;
        }
        
        PickResult::~PickResult() {
            while(!m_hits.empty()) delete m_hits.back(), m_hits.pop_back();
        }

        void PickResult::add(Hit& hit) {
            m_hits.push_back(&hit);
        }

        Hit* PickResult::first(int typeMask, bool ignoreOccluders) {
            if (!m_hits.empty()) {
                if (!m_sorted) sortHits();
                if (!ignoreOccluders) {
                    if (m_hits[0]->hasType(typeMask)) return m_hits[0];

                    float closest = m_hits[0]->distance();
                    for (unsigned int i = 1; i < m_hits.size() && m_hits[i]->distance() == closest; i++)
                        if (m_hits[i]->hasType(typeMask)) return m_hits[i];
                } else {
                    for (unsigned int i = 0; i < m_hits.size(); i++)
                        if (m_hits[i]->hasType(typeMask)) return m_hits[i];
                }
            }
            return NULL;
        }

        HitList PickResult::hits(int typeMask) {
            HitList result;
            if (!m_sorted) sortHits();
            for (unsigned int i = 0; i < m_hits.size(); i++)
                if (m_hits[i]->hasType(typeMask))
                    result.push_back(m_hits[i]);
            return result;
        }

        const HitList& PickResult::hits() {
            if (!m_sorted) sortHits();
            return m_hits;
        }

        Picker::Picker(Octree& octree) : m_octree(octree) {}

        PickResult* Picker::pick(const Ray& ray, Filter& filter) {
            PickResult* pickResults = new PickResult();

            MapObjectList objects = m_octree.intersect(ray);
            for (unsigned int i = 0; i < objects.size(); i++)
                objects[i]->pick(ray, *pickResults, filter);

            return pickResults;
        }

    }
}