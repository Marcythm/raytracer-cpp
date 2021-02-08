use crate::utilities::{p3d::P3d, vec3::Vec3};
use crate::ray::Ray;
use super::{HitRecord, Hittable};

#[derive(Debug, Clone, Copy, Default)]
pub struct Sphere {
    center: P3d,
    radius: f64,
}

impl Sphere {
    pub fn new(center: P3d, radius: f64) -> Self {
        Self { center, radius }
    }
}

impl Hittable for Sphere {
    fn hit(&self, ray: &Ray, t_min: f64, t_max: f64) -> Option<HitRecord> {
        let oc = ray.origin() - self.center;
        let a = ray.direction().length2();
        let half_b = Vec3::dot(&oc, &ray.direction());
        let c = oc.length2() - self.radius * self.radius;
        let discriminant = half_b * half_b - a * c;

        if discriminant > 0.0 {
            let root = discriminant.sqrt();

            let t = (-half_b - root) / a;
            if t_min < t && t < t_max {
                let ray_at = ray.at(t);
                let mut rec = HitRecord::new(ray_at, (ray_at - self.center) / self.radius, t);
                rec.set_face_normal(&ray);
                return Some(rec);
            }

            let t = (-half_b + root) / a;
            if t_min < t && t < t_max {
                let ray_at = ray.at(t);
                let mut rec = HitRecord::new(ray_at, (ray_at - self.center) / self.radius, t);
                rec.set_face_normal(&ray);
                return Some(rec);
            }
        }

        None
    }
}
