using UnityEngine;

public class PlayerShooting : MonoBehaviour
{
    public GameObject bulletPrefab;
    public Transform firePoint;
    public float fireRate = 8f;
    public float bulletSpeed = 14f;

    float timer;
    void Update()
    {
        if (!Input.GetButton("Fire1"))
        {
            timer = 0f;
            return; 
        }
        timer += Time.deltaTime;
        float cd = 1f / fireRate;
        while (timer >= cd)
        {
            timer -= cd;
            ShootOne(); 
        }
    }
    void ShootOne()
    {
        var go = Instantiate(bulletPrefab, firePoint.position, bulletPrefab.transform.rotation);
        var rb = go.GetComponent<Rigidbody2D>();
        if (rb)
        {
            rb.linearVelocity = Vector2.up * bulletSpeed;
        }
    }

}
