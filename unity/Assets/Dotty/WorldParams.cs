using System.Collections;
using System.Collections.Generic;
using UnityEngine;

#if UNITY_EDITOR
using UnityEditor;
#endif

namespace Dotty{
    public class WorldParams : MonoBehaviour
    {
        private bool created;
        unsafe private WorldParamsNtv* ptr; 

        public World world; 

        [Header("Timing & Performance")]
        public float timestep = 0.0166666f; 
        public int substeps = 1; 

        [Header("Global Environment")]
        [Range(0.0f, 1.0f)]
        public float globalDamping = 0.2f;
        public float gravity = 9.81f ; 

        [Header("Floor Boundry")]
        public bool hasCollisionFloor = false; 
        public float collisionFloorHeight = 0; 
        public float collisionFloorKineticFriction = 0; 
        public float collisionFloorStaticFriction = 0; 
        
        void UpdateInternal() {
            unsafe {
                ptr = world.GetWorldParamsPtr(); 
                (*ptr).timestep = timestep;
                (*ptr).substeps = substeps;
                (*ptr).globalDamping = globalDamping;
                (*ptr).gravity = gravity;
                (*ptr).hasCollisionFloor = hasCollisionFloor;
                (*ptr).collisionFloorHeight = collisionFloorHeight;
                (*ptr).collisionFloorKineticFriction = collisionFloorKineticFriction; 
                (*ptr).collisionFloorStaticFriction = collisionFloorStaticFriction; 
            }
        }

        // Start is called before the first frame update
        void Start()
        {
            if(world == null){
                world = World.instance; 
            }

            if(world == null){
                return; 
            }

            world.worldDestroyed.AddListener(OnWorldDestroyed);

            created = true; 

            UpdateInternal(); 
        }

        // Update is called once per frame
        void Update(){
            if(!created){
                return; 
            }

            UpdateInternal(); 
        }

        void OnWorldDestroyed(){
            created = false; 
        }

        void OnDestroy(){
            if(created){
                world.worldDestroyed.RemoveListener(OnWorldDestroyed); 
            }
        }
    }

    #if UNITY_EDITOR
    [CustomEditor(typeof(WorldParams))]
    public class WorldParamsEditor : Editor {
        public void OnSceneGUI() {
            var t = target as WorldParams;

            if(t.hasCollisionFloor){
                float h = t.collisionFloorHeight; 
                float s = HandleUtility.GetHandleSize( new Vector3(0, t.collisionFloorHeight, 0) ) * 3;

                Vector3 a =  new Vector3(-s, h, -s); 
                Vector3 b =  new Vector3(s, h, -s); 
                Vector3 c =  new Vector3(s, h, s);  
                Vector3 d =  new Vector3(-s, h, s);  

                Handles.DrawLine(a, b); 
                Handles.DrawLine(b, c); 
                Handles.DrawLine(c, d); 
                Handles.DrawLine(d, a); 


                EditorGUI.BeginChangeCheck();

                float handleSz = 1.5f;
                Vector3 targetPosition = new Vector3(0, t.collisionFloorHeight, 0); 
                Vector3 newTargetPosition = Handles.Slider(targetPosition, Vector3.up, handleSz * HandleUtility.GetHandleSize(targetPosition), Handles.ArrowHandleCap, 0.1f);

                if (EditorGUI.EndChangeCheck()) {
                    Undo.RecordObject(target, "Changed Height");
                    t.collisionFloorHeight = (newTargetPosition).y; 
                }
            }
        }
    }
    #endif
}