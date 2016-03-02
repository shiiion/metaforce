#ifndef _DNACOMMON_CMDL_HPP_
#define _DNACOMMON_CMDL_HPP_

#include <Athena/FileWriter.hpp>
#include <HECL/Frontend.hpp>
#include <HECL/Backend/GX.hpp>
#include "PAK.hpp"
#include "BlenderConnection.hpp"
#include "GX.hpp"
#include "TXTR.hpp"

namespace DataSpec
{
namespace DNACMDL
{

using Mesh = HECL::BlenderConnection::DataStream::Mesh;

struct Header : BigDNA
{
    DECL_DNA
    Value<atUint32> magic;
    Value<atUint32> version;
    struct Flags : BigDNA
    {
        DECL_DNA
        Value<atUint32> flags = 0;
        bool shortNormals() const {return (flags & 0x2) != 0;}
        void setShortNormals(bool val) {flags &= ~0x2; flags |= val << 1;}
        bool shortUVs() const {return (flags & 0x4) != 0;}
        void setShortUVs(bool val) {flags &= ~0x4; flags |= val << 2;}
    } flags;
    Value<atVec3f> aabbMin;
    Value<atVec3f> aabbMax;
    Value<atUint32> secCount;
    Value<atUint32> matSetCount;
    Vector<atUint32, DNA_COUNT(secCount)> secSizes;
    Align<32> align;
};

struct SurfaceHeader_1 : BigDNA
{
    DECL_DNA
    Value<atVec3f> centroid;
    Value<atUint32> matIdx = 0;
    Value<atUint16> qDiv = 0x8000;
    Value<atUint16> dlSize = 0;
    Value<atUint32> unk1 = 0;
    Value<atUint32> unk2 = 0;
    Value<atUint32> aabbSz = 0;
    Value<atVec3f> reflectionNormal;
    Seek<DNA_COUNT(aabbSz), Athena::Current> seek2;
    Align<32> align;

    static constexpr bool UseMatrixSkinning() {return false;}
    static constexpr atInt16 skinMatrixBankIdx() {return -1;}
};

struct SurfaceHeader_2 : BigDNA
{
    DECL_DNA
    Value<atVec3f> centroid;
    Value<atUint32> matIdx = 0;
    Value<atUint16> qDiv = 0x8000;
    Value<atUint16> dlSize = 0;
    Value<atUint32> unk1 = 0;
    Value<atUint32> unk2 = 0;
    Value<atUint32> aabbSz = 0;
    Value<atVec3f> reflectionNormal;
    Value<atInt16> skinMtxBankIdx;
    Value<atUint16> surfaceGroup;
    Seek<DNA_COUNT(aabbSz), Athena::Current> seek2;
    Align<32> align;

    static constexpr bool UseMatrixSkinning() {return false;}
    atInt16 skinMatrixBankIdx() const {return skinMtxBankIdx;}
};

struct SurfaceHeader_3 : BigDNA
{
    DECL_DNA
    Value<atVec3f> centroid;
    Value<atUint32> matIdx = 0;
    Value<atUint16> qDiv = 0x8000;
    Value<atUint16> dlSize = 0;
    Value<atUint32> unk1 = 0;
    Value<atUint32> unk2 = 0;
    Value<atUint32> aabbSz = 0;
    Value<atVec3f> reflectionNormal;
    Value<atInt16> skinMtxBankIdx;
    Value<atUint16> surfaceGroup;
    Seek<DNA_COUNT(aabbSz), Athena::Current> seek2;
    Value<atUint8> unk3;
    Align<32> align;

    static constexpr bool UseMatrixSkinning() {return true;}
    atInt16 skinMatrixBankIdx() const {return skinMtxBankIdx;}
};

struct VertexAttributes
{
    GX::AttrType pos = GX::NONE;
    GX::AttrType norm = GX::NONE;
    GX::AttrType color0 = GX::NONE;
    GX::AttrType color1 = GX::NONE;
    unsigned uvCount = 0;
    GX::AttrType uvs[7] = {GX::NONE};
    GX::AttrType pnMtxIdx = GX::NONE;
    unsigned texMtxIdxCount = 0;
    GX::AttrType texMtxIdx[7] = {GX::NONE};
    bool shortUVs;
};

template <class MaterialSet>
void GetVertexAttributes(const MaterialSet& matSet,
                         std::vector<VertexAttributes>& attributesOut);

template <class PAKRouter, class MaterialSet>
void ReadMaterialSetToBlender_1_2(HECL::BlenderConnection::PyOutStream& os,
                                  const MaterialSet& matSet,
                                  const PAKRouter& pakRouter,
                                  const typename PAKRouter::EntryType& entry,
                                  unsigned setIdx);

template <class PAKRouter, class MaterialSet>
void ReadMaterialSetToBlender_3(HECL::BlenderConnection::PyOutStream& os,
                                const MaterialSet& matSet,
                                const PAKRouter& pakRouter,
                                const typename PAKRouter::EntryType& entry,
                                unsigned setIdx);

void InitGeomBlenderContext(HECL::BlenderConnection::PyOutStream& os,
                            const HECL::ProjectPath& masterShaderPath);
void FinishBlenderMesh(HECL::BlenderConnection::PyOutStream& os,
                       unsigned matSetCount, int meshIdx);

template <class PAKRouter, class MaterialSet, class RigPair, class SurfaceHeader>
atUint32 ReadGeomSectionsToBlender(HECL::BlenderConnection::PyOutStream& os,
                                   Athena::io::IStreamReader& reader,
                                   PAKRouter& pakRouter,
                                   const typename PAKRouter::EntryType& entry,
                                   const RigPair& rp,
                                   bool shortNormals,
                                   bool shortUVs,
                                   std::vector<VertexAttributes>& vertAttribs,
                                   int meshIdx,
                                   atUint32 secCount,
                                   atUint32 matSetCount,
                                   const atUint32* secSizes,
                                   atUint32 surfaceCount=0);

template <class PAKRouter, class MaterialSet, class RigPair, class SurfaceHeader, atUint32 Version>
bool ReadCMDLToBlender(HECL::BlenderConnection& conn,
                       Athena::io::IStreamReader& reader,
                       PAKRouter& pakRouter,
                       const typename PAKRouter::EntryType& entry,
                       const SpecBase& dataspec,
                       const RigPair& rp);

template <class PAKRouter, class MaterialSet>
void NameCMDL(Athena::io::IStreamReader& reader,
              PAKRouter& pakRouter,
              typename PAKRouter::EntryType& entry,
              const SpecBase& dataspec);

template <class MaterialSet, class SurfaceHeader, atUint32 Version>
bool WriteCMDL(const HECL::ProjectPath& outPath, const HECL::ProjectPath& inPath, const Mesh& mesh);

template <class MaterialSet, class SurfaceHeader, atUint32 Version>
bool WriteHMDLCMDL(const HECL::ProjectPath& outPath, const HECL::ProjectPath& inPath, const Mesh& mesh);

}
}

#endif // _DNACOMMON_CMDL_HPP_
