using System.Collections;
using System.Collections.Generic;
using UnityEngine;

#if UNITY_EDITOR
using UnityEditor;
#endif

namespace Dotty {
    public class StrangeAttractor : BaseComponent{
        unsafe private StrangeAttractorNtv* ptr; 

        public Mask layerMask = (Mask)0xFF; 

        public float scale = 1; 
        public StrangeAttractorType type = StrangeAttractorType.Aizawa; 
        public float strength = 1; 
        public float targetSpeed = 1; 
        public FieldMode fieldMode = FieldMode.CorrectionForce; 
        public float minDistance = 0.1f; 
        public float maxDistance = 1000; 
        public Falloff falloff = Falloff.InvDist2; 


        float a; 
        float b; 
        float c; 
        float d; 
        float e; 
        float f; 

        override protected void AddInternal() {
            Vector3 position = transform.position; 
            internalId = world.AddStrangeAttractor(position, scale, type, strength, targetSpeed, fieldMode, minDistance, maxDistance, falloff, a, b, c, d, e, f); 
            unsafe {
                ptr = world.GetStrangeAttractorPtr(internalId); 
            }
        }
        
        override protected void RemoveInternal() {
            world.DestroyStrangeAttractor(internalId); 
        }

        // Update is called once per frame
        void Update() {
            if(!created)
                return; 
                
            Vector3 position = transform.position; 
            unsafe{
                (*ptr).layerMask = (uint)layerMask; 
                (*ptr).position.x = position.x;
                (*ptr).position.y = position.y;
                (*ptr).position.z = position.z;
                (*ptr).scale = scale; 
                (*ptr).type = type; 
                (*ptr).strength = strength; 
                (*ptr).targetSpeed = targetSpeed; 
                (*ptr).fieldMode = fieldMode; 
                (*ptr).minDist = minDistance; 
                (*ptr).maxDist = maxDistance; 
                (*ptr).falloff = falloff; 
                (*ptr).a = a; 
                (*ptr).b = b; 
                (*ptr).c = c; 
                (*ptr).d = d; 
                (*ptr).e = e; 
                (*ptr).f = f; 
            } 
        }
    }

    #if UNITY_EDITOR
    [CustomEditor(typeof(StrangeAttractor))]
    public class StrangeAttractorEditor : Editor {

        void DrawWireSphere(Vector3 center, float radius) {
            Handles.DrawWireDisc(center, new Vector3(1, 0, 0), radius); 
            Handles.DrawWireDisc(center, new Vector3(0, 1, 0), radius); 
            Handles.DrawWireDisc(center, new Vector3(0, 0, 1), radius); 
        }

        public void OnSceneGUI() {
            var t = target as StrangeAttractor;


            EditorGUI.BeginChangeCheck();

            Handles.color = Color.white;
            float maxDistance = Handles.RadiusHandle(Quaternion.identity, t.transform.position, t.maxDistance);

            if (EditorGUI.EndChangeCheck()) {
                Undo.RecordObject(target, "Changed Max Distance");
                t.maxDistance = maxDistance;
            }


            EditorGUI.BeginChangeCheck();
            
            Handles.color = Color.red;
            DrawWireSphere(t.transform.position, t.minDistance); 

            Handles.color = Color.white;

            Vector3 normal = t.transform.rotation * Vector3.up;
            float s = 2 * HandleUtility.GetHandleSize( t.transform.position ); 

            Handles.DrawDottedLine(t.transform.position, t.transform.position + new Vector3(s, 0, 0), 4);
            Handles.DrawDottedLine(t.transform.position, t.transform.position + new Vector3(-s, 0, 0), 4);

            Handles.DrawDottedLine(t.transform.position, t.transform.position + new Vector3(0, s, 0), 4);
            Handles.DrawDottedLine(t.transform.position, t.transform.position + new Vector3(0, -s, 0), 4);

            Handles.DrawDottedLine(t.transform.position, t.transform.position + new Vector3(0, 0, s), 4);
            Handles.DrawDottedLine(t.transform.position, t.transform.position + new Vector3(0, 0, -s), 4);
        }
    }
    #endif
}
