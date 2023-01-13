using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

namespace Dotty{
    public class BoxCollide : MonoBehaviour {
        private int internalId; 
        private bool started; 
        unsafe private BoxColliderNtv* ptr; 
        
        public World world; 


        public Vector3 size; 
        public float staticFriction; 
        public float kineticFriction; 
        public bool inverse; 

        void AddInternal() {
            Vector3 position = transform.position; 
            Matrix4x4 invRot = Matrix4x4.Rotate(transform.rotation).inverse;
            internalId = world.AddBoxCollider(position, invRot, size, kineticFriction, staticFriction, inverse); 
            unsafe {
                ptr = world.GetBoxColliderPtr(internalId); 
            }
        }
        
        void RemoveInternal() {
            world.DestroyBoxCollider(internalId); 
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
        void Update()
        {
            Vector3 position = transform.position; 
            Matrix4x4 invRot = Matrix4x4.Rotate(transform.rotation).inverse;
            unsafe{
                (*ptr).position.x = position.x;
                (*ptr).position.y = position.y;
                (*ptr).position.z = position.z; 
                
            } 
        }
    }

    [CustomEditor(typeof(BoxCollider))]
    public class BoxCollideEditor : Editor
    {
        public void OnSceneGUI()
        {
            var t = target as BoxCollider;



        }
    }
}