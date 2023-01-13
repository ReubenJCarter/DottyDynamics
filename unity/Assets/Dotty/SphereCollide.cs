using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor; 

namespace Dotty{
    public class SphereCollide : MonoBehaviour {
        private int internalId; 
        private bool started; 
        unsafe private SphereColliderNtv* ptr; 
        public World world;

        public float radius = 0.5f;
        public float staticFriction = 0; 
        public float kineticFriction = 0; 
        public bool inverse = false;
        
         void AddInternal() {
            Vector3 position = transform.position; 
            internalId = world.AddSphereCollider(position, radius, kineticFriction, staticFriction, inverse); 
            unsafe {
                ptr = world.GetSphereColliderPtr(internalId); 
            }
        }
        
        void RemoveInternal() {
            world.DestroySphereCollider(internalId); 
        }

        // Start is called before the first frame update
        void Start() {
            if(world == null){
                world = World.instance; 
            }
            
            started = true; 
            AddInternal(); 
        }

        void OnEnable() {
            if(started) {
                AddInternal(); 
            }
        }

        void OnDisable() {
            RemoveInternal();
        }

        // Update is called once per frame
        void Update() {
            Vector3 position = transform.position; 
            unsafe{
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
}