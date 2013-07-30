/*
 Copyright (C) 2010-2013 Kristian Duske
 
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

#include "Bsp29Model.h"

#include "CollectionUtils.h"
#include "Assets/AutoTexture.h"
#include "Renderer/Mesh.h"
#include "Renderer/MeshRenderer.h"
#include "Renderer/Vertex.h"
#include "Renderer/VertexSpec.h"

namespace TrenchBroom {
    namespace Assets {
        Bsp29Model::Face::Face(AutoTexturePtr texture) :
        m_texture(texture) {}
        
        void Bsp29Model::Face::addVertex(const Vec3f& vertex, const Vec2f& texCoord) {
            m_vertices.push_back(vertex);
            m_texCoords.push_back(texCoord);
        }

        AutoTexture* Bsp29Model::Face::texture() const {
            return m_texture.get();
        }
        
        Renderer::VertexSpecs::P3T2::Vertex::List Bsp29Model::Face::vertices() const {
            typedef Renderer::VertexSpecs::P3T2::Vertex Vertex;
            const size_t vertexCount = m_vertices.size();
            
            Renderer::VertexSpecs::P3T2::Vertex::List result;
            result.reserve(3 * (vertexCount - 2));

            for (size_t i = 1; i < vertexCount - 1; ++i) {
                result.push_back(Vertex(m_vertices[0], m_texCoords[0]));
                result.push_back(Vertex(m_vertices[i], m_texCoords[i]));
                result.push_back(Vertex(m_vertices[i+1], m_texCoords[i+1]));
            }
            return result;
        }

        Bsp29Model::SubModel::SubModel(const FaceList& i_faces, const BBox3f& i_bounds) :
        faces(i_faces),
        bounds(i_bounds) {}

        Bsp29Model::Bsp29Model(const String& name) :
        m_name(name) {}

        void Bsp29Model::addModel(const FaceList& faces, const BBox3f& bounds) {
            m_subModels.push_back(SubModel(faces, bounds));
        }

        Renderer::MeshRenderer* Bsp29Model::doBuildRenderer(Renderer::Vbo& vbo, const size_t skinIndex, const size_t frameIndex) const {
            
            Renderer::Mesh<const Assets::Texture*, Renderer::VertexSpecs::P3T2> mesh;

            const SubModel& model = m_subModels.front();
            FaceList::const_iterator it, end;
            for (it = model.faces.begin(), end = model.faces.end(); it != end; ++it) {
                const Face& face = *it;
                mesh.beginTriangleSet(face.texture());
                mesh.addTrianglesToSet(face.vertices());
                mesh.endTriangleSet();
            }
            
            return new Renderer::MeshRenderer(vbo, mesh);
        }

        BBox3f Bsp29Model::doGetBounds(const size_t skinIndex, const size_t frameIndex) const {
            const SubModel& model = m_subModels.front();
            return model.bounds;
        }
    }
}
