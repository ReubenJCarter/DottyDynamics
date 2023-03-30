using System.Collections;
using System.Collections.Generic;
using UnityEngine;

#if UNITY_EDITOR
using UnityEditor;
#endif

namespace Dotty{
    public class SphereCollide : BaseComponent { 
        unsafe private SphereColliderNtv* ptr; 

        public Mask layerMask = (Mask)0xFF; 

        public float radius = 0.5f;
        public float staticFriction = 0; 
        public float kineticFriction = 0; 
        public bool inverse = false;
        
        override protected void AddInternal() {
            Vector3 position = transform.position; 
            internalId = world.AddSphereCollider(position, radius, kineticFriction, staticFriction, inverse); 
            unsafe {
                ptr = world.GetSphereColliderPtr(internalId); 
            }
        }
        
        override protected void RemoveInternal() {
            world.DestroySphereCollider(internalId); 
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
                (*ptr).radius = radius; 
                (*ptr).kineticFriction = kineticFriction; 
                (*ptr).staticFriction = staticFriction; 
                (*ptr).inverse = inverse; 
            } 
        }
    }

    #if UNITY_EDITOR
    [CustomEditor(typeof(SphereCollide))]
    public class SphereCollideEditor : Editor {
        public void OnSceneGUI() {
            var t = target as SphereCollide;

            EditorGUI.BeginChangeCheck();

            Handles.color = Color.white;
            float radius = Handles.RadiusHandle(Quaternion.identity, t.transform.position, t.radius);

            if (EditorGUI.EndChangeCheck()){
                Undo.RecordObject(target, "Changed Radius");
                t.radius = radius;
            }
        }
    }
    #endif
}