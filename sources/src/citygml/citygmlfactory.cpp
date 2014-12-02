#include "citygml/citygmlfactory.h"
#include "citygml/appearancemanager.h"
#include "citygml/polygonmanager.h"
#include "citygml/cityobject.h"
#include "citygml/appearancetarget.h"
#include "citygml/polygon.h"
#include "citygml/implictgeometry.h"
#include "citygml/texture.h"
#include "citygml/georeferencedtexture.h"
#include "citygml/material.h"
#include "citygml/materialtargetdefinition.h"
#include "citygml/texturetargetdefinition.h"
#include "citygml/citymodel.h"
#include "citygml/implictgeometry.h"
#include "citygml/citygmllogger.h"

namespace citygml {

    CityGMLFactory::CityGMLFactory(std::shared_ptr<CityGMLLogger> logger)
    {
        m_appearanceManager = std::unique_ptr<AppearanceManager>(new AppearanceManager(logger));
        m_polygonManager = std::unique_ptr<PolygonManager>(new PolygonManager(logger));
        m_logger = logger;
    }

    CityModel* CityGMLFactory::createCityModel(const std::string& id)
    {
        return new CityModel(id);
    }

    CityObject* CityGMLFactory::createCityObject(const std::string& id, CityObject::CityObjectsType type)
    {
        CityObject* cityObject = new CityObject(id, type);
        return cityObject;
    }

    Geometry* CityGMLFactory::createGeometry(const std::string& id, Geometry::GeometryType type, unsigned int lod)
    {
        Geometry* geom = new Geometry(id, type, lod);
        appearanceTargetCreated(geom);
        return geom;
    }

    std::shared_ptr<Polygon> CityGMLFactory::createPolygon(const std::string& id)
    {
        Polygon* poly = new Polygon(id, m_logger);
        appearanceTargetCreated(poly);

        std::shared_ptr<Polygon> shared = std::shared_ptr<Polygon>(poly);
        m_polygonManager->addPolygon(shared);

        return shared;
    }

    void CityGMLFactory::requestSharedPolygonForGeometry(Geometry* geom, const std::string& polygonId)
    {
        m_polygonManager->requestSharedPolygonForGeometry(geom, polygonId);
    }

    ImplicitGeometry *CityGMLFactory::createImplictGeometry(const std::string& id)
    {
        return new ImplicitGeometry(id);
    }

    std::shared_ptr<Geometry> CityGMLFactory::shareGeometry(Geometry* geom)
    {
        std::shared_ptr<Geometry> shared = getSharedGeometryWithID(geom->getId());

        if (shared == nullptr) {
            shared = std::shared_ptr<Geometry>(geom);
        } else if (shared != nullptr && geom != shared.get()) {
            throw std::runtime_error("CityGMLFactory::sharedGeometry called for two different object with the same gml:id... using first shared geometry, delete other geometry");
        }

        m_sharedGeometriesMap[geom->getId()] = shared;
        return shared;
    }

    std::shared_ptr<Geometry> CityGMLFactory::getSharedGeometryWithID(const std::string& id)
    {
        auto it = m_sharedGeometriesMap.find(id);
        if (it == m_sharedGeometriesMap.end()) {
            return nullptr;
        }
        return it->second;
    }

    std::shared_ptr<Texture> CityGMLFactory::createTexture(const std::string& id)
    {
        std::shared_ptr<Texture> tex = std::shared_ptr<Texture>(new Texture(id));
        m_appearanceManager->addAppearance(tex);
        return tex;
    }

    std::shared_ptr<Material> CityGMLFactory::createMaterial(const std::string& id)
    {
        std::shared_ptr<Material> mat = std::shared_ptr<Material>(new Material(id));
        m_appearanceManager->addAppearance(mat);
        return mat;
    }

    std::shared_ptr<GeoreferencedTexture> CityGMLFactory::createGeoReferencedTexture(const std::string& id)
    {
        std::shared_ptr<GeoreferencedTexture> tex = std::shared_ptr<GeoreferencedTexture>(new GeoreferencedTexture(id));
        m_appearanceManager->addAppearance(tex);
        return tex;
    }

    std::shared_ptr<MaterialTargetDefinition> CityGMLFactory::createMaterialTargetDefinition(const std::string& targetID, std::shared_ptr<Material> appearance, const std::string& id)
    {
        std::shared_ptr<MaterialTargetDefinition> targetDef = std::shared_ptr<MaterialTargetDefinition>(new MaterialTargetDefinition(targetID, appearance, id));
        m_appearanceManager->addMaterialTargetDefinition(targetDef);
        return targetDef;
    }

    std::shared_ptr<TextureTargetDefinition> CityGMLFactory::createTextureTargetDefinition(const std::string& targetID, std::shared_ptr<Texture> appearance, const std::string& id)
    {
        std::shared_ptr<TextureTargetDefinition> targetDef = std::shared_ptr<TextureTargetDefinition>(new TextureTargetDefinition(targetID, appearance, id));
        m_appearanceManager->addTextureTargetDefinition(targetDef);
        return targetDef;
    }

    std::shared_ptr<Appearance> CityGMLFactory::getAppearanceWithID(const std::string& id)
    {
        return m_appearanceManager->getAppearanceByID(id);
    }

    std::vector<std::string> CityGMLFactory::getAllThemes()
    {
        return m_appearanceManager->getAllThemes();
    }

    void CityGMLFactory::closeFactory()
    {
        m_polygonManager->finish();
        m_appearanceManager->assignAppearancesToTargets();
    }

    CityGMLFactory::~CityGMLFactory()
    {

    }

    void CityGMLFactory::appearanceTargetCreated(AppearanceTarget* obj)
    {
        m_appearanceManager->addAppearanceTarget(obj);
    }
}
