#ifndef _DNAMP1_ANCS_HPP_
#define _DNAMP1_ANCS_HPP_

#include "../DNACommon/DNACommon.hpp"

namespace Retro
{
namespace DNAMP1
{

struct ANCS : BigYAML
{
    DECL_YAML
    Value<atUint16> version;

    struct CharacterSet : BigYAML
    {
        DECL_YAML
        Value<atUint16> version;
        Value<atUint32> characterCount;
        struct CharacterInfo : BigYAML
        {
            Delete expl;

            atUint32 idx;
            std::string name;
            UniqueID32 cmdl;
            UniqueID32 cskr;
            UniqueID32 cinf;

            struct Animation : BigYAML
            {
                DECL_YAML
                Value<atUint32> animIdx;
                String<-1> strA;
                String<-1> strB;
            };
            std::vector<Animation> animations;

            struct PASDatabase : BigYAML
            {
                DECL_YAML
                Value<atUint32> magic;
                Value<atUint32> animStateCount;
                Value<atUint32> defaultState;
                struct AnimState : BigYAML
                {
                    Delete expl;
                    atUint32 id;

                    struct ParmInfo : BigYAML
                    {
                        Delete expl;
                        enum DataType
                        {
                            DTInt32 = 0,
                            DTUInt32 = 1,
                            DTFloat = 2,
                            DTBool = 3,
                            DTEnum = 4
                        };
                        union Parm
                        {
                            atInt32 int32;
                            atUint32 uint32;
                            float float32;
                            bool bool1;
                            Parm() : int32(0) {}
                            Parm(atInt32 val) : int32(val) {}
                            Parm(atUint32 val) : uint32(val) {}
                            Parm(float val) : float32(val) {}
                            Parm(bool val) : bool1(val) {}
                        };

                        atUint32 parmType;
                        atUint32 unk1;
                        float unk2;
                        Parm parmVals[2];

                        void read(Athena::io::IStreamReader& reader);
                        void write(Athena::io::IStreamWriter& writer) const;
                        void fromYAML(Athena::io::YAMLDocReader& reader);
                        void toYAML(Athena::io::YAMLDocWriter& writer) const;
                    };
                    std::vector<ParmInfo> parmInfos;

                    struct AnimInfo
                    {
                        atUint32 id;
                        std::vector<ParmInfo::Parm> parmVals;
                    };
                    std::vector<AnimInfo> animInfos;

                    void read(Athena::io::IStreamReader& reader);
                    void write(Athena::io::IStreamWriter& writer) const;
                    void fromYAML(Athena::io::YAMLDocReader& reader);
                    void toYAML(Athena::io::YAMLDocWriter& writer) const;
                };
            } pasDatabase;

            struct ParticleResData
            {
                std::vector<UniqueID32> part;
                std::vector<UniqueID32> swhc;
                std::vector<UniqueID32> unk;
                std::vector<UniqueID32> elsc;
            } partResData;

            atUint32 unk1;

            struct ActionAABB : BigYAML
            {
                DECL_YAML
                String<-1> name;
                Value<atVec3f> aabb[2];
            };
            std::vector<ActionAABB> animAABBs;

            struct Effect : BigYAML
            {
                DECL_YAML
                String<-1> name;
                Value<atUint32> compCount;
                struct EffectComponent : BigYAML
                {
                    DECL_YAML
                    String<-1> name;
                    FourCC type;
                    UniqueID32 id;
                    String<-1> name2;
                    Value<float> unk1;
                    Value<atUint32> unk2;
                    Value<atUint32> unk3;
                };
                Vector<EffectComponent, DNA_COUNT(compCount)> comps;
            };
            std::vector<Effect> effects;

            UniqueID32 cmdlOverride;
            UniqueID32 cskrOverride;

            std::vector<atUint32> animIdxs;

            void read(Athena::io::IStreamReader& reader);
            void write(Athena::io::IStreamWriter& writer) const;
            void fromYAML(Athena::io::YAMLDocReader& reader);
            void toYAML(Athena::io::YAMLDocWriter& writer) const;

        };
    } characterSet;

    struct AnimationSet : BigYAML
    {
        Delete expl;

        struct IMetaAnim : BigYAML
        {
            Delete expl;
            void read(Athena::io::IStreamReader&) {}
            void write(Athena::io::IStreamWriter&) const {}
            void fromYAML(Athena::io::YAMLDocReader&) {}
            void toYAML(Athena::io::YAMLDocWriter&) const {}
            enum Type
            {
                MAPrimitive = 0,
                MABlend = 1,
                MAPhaseBlend = 2,
                MARandom = 3,
                MASequence = 4
            } m_type;
            const char* m_typeStr;
            IMetaAnim(Type type, const char* typeStr)
            : m_type(type), m_typeStr(typeStr) {}
        };
        struct MetaAnimFactory : BigYAML
        {
            Delete expl;
            std::unique_ptr<IMetaAnim> m_anim;
            void read(Athena::io::IStreamReader& reader);
            void write(Athena::io::IStreamWriter& writer) const;
            void fromYAML(Athena::io::YAMLDocReader& reader);
            void toYAML(Athena::io::YAMLDocWriter& writer) const;
        };
        struct MetaAnimPrimitive : IMetaAnim
        {
            MetaAnimPrimitive() : IMetaAnim(MAPrimitive, "Primitive") {}
            DECL_YAML
            UniqueID32 animId;
            Value<atUint32> animIdx;
            String<-1> animName;
            Value<float> unk1;
            Value<atUint32> unk2;
        };
        struct MetaAnimBlend : IMetaAnim
        {
            MetaAnimBlend() : IMetaAnim(MABlend, "Blend") {}
            DECL_YAML
            MetaAnimFactory animA;
            MetaAnimFactory animB;
            Value<float> unkFloat;
            Value<atUint8> unk;
        };
        struct MetaAnimPhaseBlend : IMetaAnim
        {
            MetaAnimPhaseBlend() : IMetaAnim(MAPhaseBlend, "PhaseBlend") {}
            DECL_YAML
            MetaAnimFactory animA;
            MetaAnimFactory animB;
            Value<float> unkFloat;
            Value<atUint8> unk;
        };
        struct MetaAnimRandom : IMetaAnim
        {
            MetaAnimRandom() : IMetaAnim(MARandom, "Random") {}
            DECL_YAML
            Value<atUint32> animCount;
            struct Child : BigYAML
            {
                DECL_YAML
                MetaAnimFactory anim;
                Value<atUint32> probability;
            };
            Vector<Child, DNA_COUNT(animCount)> children;
        };
        struct MetaAnimSequence : IMetaAnim
        {
            MetaAnimSequence() : IMetaAnim(MASequence, "Sequence") {}
            DECL_YAML
            Value<atUint32> animCount;
            Vector<MetaAnimFactory, DNA_COUNT(animCount)> children;
        };

        struct Animation : BigYAML
        {
            DECL_YAML
            String<-1> name;
            MetaAnimFactory metaAnim;
        };
        std::vector<Animation> animations;

        struct IMetaTrans : BigYAML
        {
            Delete expl;
            void read(Athena::io::IStreamReader&) {}
            void write(Athena::io::IStreamWriter&) const {}
            void fromYAML(Athena::io::YAMLDocReader&) {}
            void toYAML(Athena::io::YAMLDocWriter&) const {}
            enum Type
            {
                MTMetaAnim = 0,
                MTTrans = 1,
                MTPhaseTrans = 2,
                MTNoTrans = 3,
            } m_type;
            const char* m_typeStr;
            IMetaTrans(Type type, const char* typeStr)
            : m_type(type), m_typeStr(typeStr) {}
        };
        struct MetaTransFactory : BigYAML
        {
            Delete expl;
            std::unique_ptr<IMetaTrans> m_trans;
            void read(Athena::io::IStreamReader& reader);
            void write(Athena::io::IStreamWriter& writer) const;
            void fromYAML(Athena::io::YAMLDocReader& reader);
            void toYAML(Athena::io::YAMLDocWriter& writer) const;
        };
        struct MetaTransMetaAnim : IMetaTrans
        {
            MetaTransMetaAnim() : IMetaTrans(MTMetaAnim, "MetaAnim") {}
            DECL_YAML
            MetaAnimFactory anim;
        };
        struct MetaTransTrans : IMetaTrans
        {
            MetaTransTrans() : IMetaTrans(MTTrans, "Trans") {}
            DECL_YAML
            Value<float> time;
            Value<atUint32> unk1;
            Value<atUint8> unk2;
            Value<atUint8> unk3;
            Value<atUint32> unk4;

        };
        struct MetaTransPhaseTrans : IMetaTrans
        {
            MetaTransPhaseTrans() : IMetaTrans(MTPhaseTrans, "PhaseTrans") {}
            DECL_YAML
            Value<float> time;
            Value<atUint32> unk1;
            Value<atUint8> unk2;
            Value<atUint8> unk3;
            Value<atUint32> unk4;
        };

        struct Transition : BigYAML
        {
            DECL_YAML
            Value<atUint32> unk;
            Value<atUint32> animIdxA;
            Value<atUint32> animIdxB;
            MetaTransFactory metaTrans;
        };
        std::vector<Transition> transitions;

        struct AdditiveAnimationInfo : BigYAML
        {
            DECL_YAML
            Value<atUint32> animIdx;
            Value<float> unk1;
            Value<float> unk2;
        };
        std::vector<AdditiveAnimationInfo> additiveAnims;

        float floatA = 0.0;
        float floatB = 0.0;

        struct HalfTransition : BigYAML
        {
            DECL_YAML
            Value<atUint32> animIdx;
            MetaTransFactory metaTrans;
        };
        std::vector<HalfTransition> halfTransitions;

        struct AnimationResources : BigYAML
        {
            DECL_YAML
            UniqueID32 animId;
            UniqueID32 evntId;
        };
        std::vector<AnimationResources> animResources;

        void read(Athena::io::IStreamReader& reader);
        void write(Athena::io::IStreamWriter& writer) const;
        void fromYAML(Athena::io::YAMLDocReader& reader);
        void toYAML(Athena::io::YAMLDocWriter& writer) const;
    } animationSet;
};

}
}

#endif // _DNAMP1_ANCS_HPP_
