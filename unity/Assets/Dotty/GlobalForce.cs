using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor; 

namespace Dotty {
    public class GlobalForce : MonoBehaviour
    {
        private int internalId; 
        private bool started; 
        unsafe private GlobalForceNtv* ptr; 
        
        public World world; 

        public Vector3 direction;
        public float strength; 
        public Vector3 boundSize; 
        public BoundShapeType boundShape;
        public float boundThickness; 
        public Falloff boundFalloff;

        void AddInternal() {
            Vector3 position = transform.position; 
            Matrix4x4 invRot = Matrix4x4.Rotate(transform.rotation).inverse;
            internalId = world.AddGlobalForce(position, direction, strength, boundSize, boundShape, boundThickness, boundFalloff, invRot); 
            unsafe {
                ptr = world.GetGlobalForcePtr(internalId); 
            }
        }
        
        void RemoveInternal() {
            world.DestroyGlobalForce(internalId); 
        }

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
            Mat3 ir = new Mat3(); 
            ir.x0 = invRot[0, 0]; 
            ir.y0 = invRot[1, 0]; 
            ir.z0 = invRot[2, 0]; 
            ir.x1 = invRot[0, 1]; 
            ir.y1 = invRot[1, 1]; 
            ir.z1 = invRot[2, 1]; 
            ir.x2 = invRot[0, 2]; 
            ir.y2 = invRot[1, 2]; 
            ir.z2 = invRot[2, 2]; 

            Vec3 sz = new Vec3(); 
            sz.x = boundSize.x; 
            sz.y = boundSize.y; 
            sz.z = boundSize.z; 

            Vec3 d = new Vec3(); 
            d.x = direction.x; 
            d.y = direction.y; 
            d.z = direction.z; 

            unsafe{
                (*ptr).position.x = position.x;
                (*ptr).position.y = position.y;
                (*ptr).position.z = position.z; 
                (*ptr).direction = d;
                (*ptr).strength = strength;
                (*ptr).boundSize = sz; 
                (*ptr).boundShape = boundShape; 
                (*ptr).boundThickness = boundThickness; 
                (*ptr).boundFalloff = boundFalloff; 
                (*ptr).boundInvRotation = ir; 
            } 
        }
    }

    [CustomEditor(typeof(GlobalForce))]
    public class GlobalForceEditor : Editor {

        private void DrawWireCube(GlobalForce t){
            Vector3 a = t.transform.TransformPoint( new Vector3(-t.boundSize.x/2, -t.boundSize.y/2, -t.boundSize.z/2) ); 
            Vector3 b = t.transform.TransformPoint( new Vector3(-t.boundSize.x/2, t.boundSize.y/2, -t.boundSize.z/2) ); 
            Vector3 c = t.transform.TransformPoint( new Vector3(t.boundSize.x/2, t.boundSize.y/2, -t.boundSize.z/2) ); 
            Vector3 d = t.transform.TransformPoint( new Vector3(t.boundSize.x/2, -t.boundSize.y/2, -t.boundSize.z/2) ); 

            Vector3 e = t.transform.TransformPoint( new Vector3(-t.boundSize.x/2, -t.boundSize.y/2, t.boundSize.z/2) ); 
            Vector3 f = t.transform.TransformPoint( new Vector3(-t.boundSize.x/2, t.boundSize.y/2, t.boundSize.z/2) ); 
            Vector3 g = t.transform.TransformPoint( new Vector3(t.boundSize.x/2, t.boundSize.y/2, t.boundSize.z/2) ); 
            Vector3 h = t.transform.TransformPoint( new Vector3(t.boundSize.x/2, -t.boundSize.y/2, t.boundSize.z/2) ); 

            Handles.DrawLine(a, b);
            Handles.DrawLine(b, c);
            Handles.DrawLine(c, d);
            Handles.DrawLine(d, a);

            Handles.DrawLine(e, f);
            Handles.DrawLine(f, g);
            Handles.DrawLine(g, h);
            Handles.DrawLine(h, e);

            Handles.DrawLine(a, e);
            Handles.DrawLine(b, f);
            Handles.DrawLine(c, g);
            Handles.DrawLine(d, h);
        }

        public void OnSceneGUI() {
            var t = target as GlobalForce;

            if(t.boundShape == BoundShapeType.Sphere){



                EditorGUI.BeginChangeCheck();

                Handles.color = Color.white;
                float sz = Handles.RadiusHandle(Quaternion.identity, t.transform.position, t.boundSize.x);

                if (EditorGUI.EndChangeCheck()){
                    Undo.RecordObject(target, "Changed Size");
                    t.boundSize.x = sz;
                }


                EditorGUI.BeginChangeCheck();

                Handles.color = Color.red;
                float tkn = Handles.RadiusHandle(Quaternion.identity, t.transform.position, t.boundSize.x - t.boundThickness);

                if (EditorGUI.EndChangeCheck()){
                    Undo.RecordObject(target, "Changed Thickness");
                    t.boundThickness = t.boundSize.x - tkn;
                }



            }
            else if(t.boundShape == BoundShapeType.Box){


                Handles.color = Color.white;
                DrawWireCube(t); 

                float handleSz = 0.03f;

                //Handle x axis 
                EditorGUI.BeginChangeCheck();

                Vector3 targetPosition = t.transform.TransformPoint( new Vector3(t.boundSize.x/2, 0f, 0f) ); 
                Vector3 targetAxis = t.transform.TransformDirection( new Vector3(1, 0, 0)); 
                Vector3 newTargetPosition = Handles.Slider(targetPosition, targetAxis, handleSz * HandleUtility.GetHandleSize(targetPosition), Handles.DotHandleCap, 0.1f);
                Vector3 newSize = t.transform.InverseTransformPoint( newTargetPosition ) * 2;

                if (EditorGUI.EndChangeCheck()) {
                    Undo.RecordObject(target, "Changed Size");
                    t.boundSize.x = newSize.x; 
                }

                EditorGUI.BeginChangeCheck();
                
                targetPosition = t.transform.TransformPoint( new Vector3(-t.boundSize.x/2, 0f, 0f) ); 
                targetAxis = t.transform.TransformDirection( new Vector3(-1, 0, 0)); 
                newTargetPosition = Handles.Slider(targetPosition, targetAxis, handleSz * HandleUtility.GetHandleSize(targetPosition), Handles.DotHandleCap, 0.1f);
                newSize = t.transform.InverseTransformPoint( newTargetPosition ) * -2;

                if (EditorGUI.EndChangeCheck()) {
                    Undo.RecordObject(target, "Changed Size");
                    t.boundSize.x = newSize.x; 
                }

                //Handle y axis 
                EditorGUI.BeginChangeCheck();

                targetPosition = t.transform.TransformPoint( new Vector3(0, t.boundSize.y/2, 0f) ); 
                targetAxis = t.transform.TransformDirection( new Vector3(0, 1, 0)); 
                newTargetPosition = Handles.Slider(targetPosition, targetAxis, handleSz * HandleUtility.GetHandleSize(targetPosition), Handles.DotHandleCap, 0.1f);
                newSize = t.transform.InverseTransformPoint( newTargetPosition ) * 2;

                if (EditorGUI.EndChangeCheck()) {
                    Undo.RecordObject(target, "Changed Size");
                    t.boundSize.y = newSize.y; 
                }

                EditorGUI.BeginChangeCheck();
                
                targetPosition = t.transform.TransformPoint( new Vector3(0f, -t.boundSize.y/2, 0f) ); 
                targetAxis = t.transform.TransformDirection( new Vector3(0, -1, 0)); 
                newTargetPosition = Handles.Slider(targetPosition, targetAxis, handleSz * HandleUtility.GetHandleSize(targetPosition), Handles.DotHandleCap, 0.1f);
                newSize = t.transform.InverseTransformPoint( newTargetPosition ) * -2;

                if (EditorGUI.EndChangeCheck()) {
                    Undo.RecordObject(target, "Changed Size");
                    t.boundSize.y = newSize.y; 
                }

                //Handle z axis 
                EditorGUI.BeginChangeCheck();

                targetPosition = t.transform.TransformPoint( new Vector3(0, 0f, t.boundSize.z/2) ); 
                targetAxis = t.transform.TransformDirection( new Vector3(0, 0, 1)); 
                newTargetPosition = Handles.Slider(targetPosition, targetAxis, handleSz * HandleUtility.GetHandleSize(targetPosition), Handles.DotHandleCap, 0.1f);
                newSize = t.transform.InverseTransformPoint( newTargetPosition ) * 2;

                if (EditorGUI.EndChangeCheck()) {
                    Undo.RecordObject(target, "Changed Size");
                    t.boundSize.z = newSize.z; 
                }

                EditorGUI.BeginChangeCheck();
                
                targetPosition = t.transform.TransformPoint( new Vector3(0f, 0f, -t.boundSize.z/2) ); 
                targetAxis = t.transform.TransformDirection( new Vector3(0, 0, -1)); 
                newTargetPosition = Handles.Slider(targetPosition, targetAxis, handleSz * HandleUtility.GetHandleSize(targetPosition), Handles.DotHandleCap, 0.1f);
                newSize = t.transform.InverseTransformPoint( newTargetPosition ) * -2;

                if (EditorGUI.EndChangeCheck()) {
                    Undo.RecordObject(target, "Changed Size");
                    t.boundSize.z = newSize.z; 
                }
            }
        }
    }
}