-- ============================================================
--  ERO RIVERSIDE HÀ NỘI – BẮC NINH
--  PostgreSQL Schema  |  main.sql  (v3 - Enhanced Audit & FKs)
--  UUID primary keys auto-generated via gen_random_uuid()
-- ============================================================

CREATE EXTENSION IF NOT EXISTS pgcrypto;

-- Optional: Clean up existing tables and types for clean recreation during development
DROP TABLE IF EXISTS activity_logs, registration_interests, registrations, media, product_images, products, floor_plans, site_settings, project_info, admin_sessions, admins CASCADE;
DROP TYPE IF EXISTS product_type, media_type, log_action CASCADE;

-- ============================================================
-- 1. ENUM TYPES
-- ============================================================

CREATE TYPE product_type AS ENUM ('lien_ke', 'shophouse', 'biet_thu');
CREATE TYPE media_type   AS ENUM ('image', 'video');
CREATE TYPE log_action   AS ENUM ('login', 'logout', 'create', 'update', 'delete', 'upload');

-- ============================================================
-- 2. ADMINS
-- ============================================================

CREATE TABLE admins (
                            id            UUID         PRIMARY KEY DEFAULT gen_random_uuid(),
                            username      VARCHAR(64)  NOT NULL UNIQUE,
                            password_hash TEXT         NOT NULL,
                            full_name     VARCHAR(128),
                            is_active     BOOLEAN      NOT NULL DEFAULT TRUE,
                            created_at    TIMESTAMPTZ  NOT NULL DEFAULT NOW(),
                            updated_at    TIMESTAMPTZ  NOT NULL DEFAULT NOW()
);

-- ============================================================
-- 3. ADMIN SESSIONS
--    Quản lý phiên đăng nhập / refresh token (Phục vụ ATTT)
-- ============================================================

CREATE TABLE admin_sessions (
                                    id            UUID        PRIMARY KEY DEFAULT gen_random_uuid(),
                                    admin_id      UUID        NOT NULL CONSTRAINT fk_admin_sessions_admin REFERENCES admins (id) ON DELETE CASCADE,
                                    refresh_token TEXT        NOT NULL UNIQUE,
                                    ip_address    INET,
                                    user_agent    TEXT,
                                    expires_at    TIMESTAMPTZ NOT NULL,
                                    revoked       BOOLEAN     NOT NULL DEFAULT FALSE,
                                    created_at    TIMESTAMPTZ NOT NULL DEFAULT NOW()
);

CREATE INDEX idx_sessions_admin   ON admin_sessions (admin_id);
CREATE INDEX idx_sessions_token   ON admin_sessions (refresh_token);
CREATE INDEX idx_sessions_expires ON admin_sessions (expires_at);

-- ============================================================
-- 4. PROJECT INFO
--    Thông tin tổng quan dự án (singleton)
-- ============================================================

CREATE TABLE project_info (
                                  id          UUID             PRIMARY KEY DEFAULT gen_random_uuid(),
                                  name        VARCHAR(256)     NOT NULL,
                                  developer   VARCHAR(256),
                                  scale       TEXT,
                                  description TEXT,
                                  map_lat     DOUBLE PRECISION,
                                  map_lng     DOUBLE PRECISION,
                                  map_zoom    SMALLINT         NOT NULL DEFAULT 15,
                                  updated_by  UUID             CONSTRAINT fk_project_info_updater REFERENCES admins (id) ON DELETE SET NULL,
                                  updated_at  TIMESTAMPTZ      NOT NULL DEFAULT NOW()
);

CREATE UNIQUE INDEX idx_project_info_singleton ON project_info ((TRUE));

-- ============================================================
-- 5. SITE SETTINGS
--    Thông tin footer & liên hệ (singleton)
-- ============================================================

CREATE TABLE site_settings (
                                   id            UUID        PRIMARY KEY DEFAULT gen_random_uuid(),
                                   hotline       VARCHAR(20),
                                   email         VARCHAR(128),
                                   address       TEXT,
                                   zalo_link     TEXT,
                                   facebook_link TEXT,
                                   logo_url      TEXT,
                                   tagline       TEXT,
                                   updated_by    UUID        CONSTRAINT fk_site_settings_updater REFERENCES admins (id) ON DELETE SET NULL,
                                   updated_at    TIMESTAMPTZ NOT NULL DEFAULT NOW()
);

CREATE UNIQUE INDEX idx_site_settings_singleton ON site_settings ((TRUE));

-- ============================================================
-- 6. FLOOR PLANS
--    Sơ đồ dự án – nhiều ảnh, có nhãn phân loại
-- ============================================================

CREATE TABLE floor_plans (
                                 id          UUID         PRIMARY KEY DEFAULT gen_random_uuid(),
                                 title       VARCHAR(256) NOT NULL,
                                 url         TEXT         NOT NULL,
                                 sort_order  SMALLINT     NOT NULL DEFAULT 0,
                                 is_active   BOOLEAN      NOT NULL DEFAULT TRUE,
                                 uploaded_by UUID         CONSTRAINT fk_floor_plans_uploader REFERENCES admins (id) ON DELETE SET NULL,
                                 created_at  TIMESTAMPTZ  NOT NULL DEFAULT NOW()
);

CREATE INDEX idx_floor_plans_sort ON floor_plans (sort_order);

-- ============================================================
-- 7. PRODUCTS
--    Sản phẩm: liền kề / shophouse / biệt thự
-- ============================================================

CREATE TABLE products (
                              id          UUID           PRIMARY KEY DEFAULT gen_random_uuid(),
                              type        product_type   NOT NULL,
                              name        VARCHAR(256)   NOT NULL,
                              price       NUMERIC(18, 0) NOT NULL CHECK (price >= 0),
                              area        NUMERIC(10, 2),
                              description TEXT,
                              is_active   BOOLEAN        NOT NULL DEFAULT TRUE,
                              created_by  UUID           CONSTRAINT fk_products_creator REFERENCES admins (id) ON DELETE SET NULL,
                              updated_by  UUID           CONSTRAINT fk_products_updater REFERENCES admins (id) ON DELETE SET NULL,
                              created_at  TIMESTAMPTZ    NOT NULL DEFAULT NOW(),
                              updated_at  TIMESTAMPTZ    NOT NULL DEFAULT NOW()
);

CREATE INDEX idx_products_type  ON products (type);
CREATE INDEX idx_products_price ON products (price);

-- ============================================================
-- 8. PRODUCT IMAGES
--    Ảnh gắn với sản phẩm (1 sản phẩm – nhiều ảnh)
-- ============================================================

CREATE TABLE product_images (
                                    id          UUID        PRIMARY KEY DEFAULT gen_random_uuid(),
                                    product_id  UUID        NOT NULL CONSTRAINT fk_product_images_product REFERENCES products (id) ON DELETE CASCADE,
                                    url         TEXT        NOT NULL,
                                    is_cover    BOOLEAN     NOT NULL DEFAULT FALSE,
                                    sort_order  SMALLINT    NOT NULL DEFAULT 0,
                                    uploaded_by UUID        CONSTRAINT fk_product_images_uploader REFERENCES admins (id) ON DELETE SET NULL,
                                    created_at  TIMESTAMPTZ NOT NULL DEFAULT NOW()
);

CREATE INDEX idx_product_images_product ON product_images (product_id);

-- ============================================================
-- 9. MEDIA LIBRARY
--    Thư viện ảnh / video chung của dự án
-- ============================================================

CREATE TABLE media (
                           id          UUID        PRIMARY KEY DEFAULT gen_random_uuid(),
                           type        media_type  NOT NULL,
                           title       VARCHAR(256),
                           url         TEXT        NOT NULL,
                           thumbnail   TEXT,
                           sort_order  SMALLINT    NOT NULL DEFAULT 0,
                           is_active   BOOLEAN     NOT NULL DEFAULT TRUE,
                           uploaded_by UUID        CONSTRAINT fk_media_uploader REFERENCES admins (id) ON DELETE SET NULL,
                           created_at  TIMESTAMPTZ NOT NULL DEFAULT NOW()
);

CREATE INDEX idx_media_type   ON media (type);
CREATE INDEX idx_media_active ON media (is_active);

-- ============================================================
-- 10. REGISTRATIONS
--     Form đăng ký khách hàng
-- ============================================================

CREATE TABLE registrations (
                                   id           UUID         PRIMARY KEY DEFAULT gen_random_uuid(),
                                   full_name    VARCHAR(128) NOT NULL,
                                   phone        VARCHAR(20)  NOT NULL,
                                   email        VARCHAR(128),
                                   address      TEXT,
                                   note         TEXT,
                                   is_contacted BOOLEAN      NOT NULL DEFAULT FALSE,
                                   contacted_by UUID         CONSTRAINT fk_registrations_admin REFERENCES admins (id) ON DELETE SET NULL,
                                   contacted_at TIMESTAMPTZ,
                                   created_at   TIMESTAMPTZ  NOT NULL DEFAULT NOW()
);

CREATE INDEX idx_registrations_created   ON registrations (created_at DESC);
CREATE INDEX idx_registrations_contacted ON registrations (is_contacted);

-- ============================================================
-- 11. REGISTRATION INTERESTS
--     Nhu cầu của khách – N:1 với registrations
-- ============================================================

CREATE TABLE registration_interests (
                                            id              UUID         PRIMARY KEY DEFAULT gen_random_uuid(),
                                            registration_id UUID         NOT NULL CONSTRAINT fk_reg_interests_reg REFERENCES registrations (id) ON DELETE CASCADE,
                                            product_type    product_type NOT NULL,
                                            UNIQUE (registration_id, product_type)
);

CREATE INDEX idx_reg_interests_registration ON registration_interests (registration_id);
CREATE INDEX idx_reg_interests_type         ON registration_interests (product_type);

-- ============================================================
-- 12. ACTIVITY LOGS
--     Ghi log: login/logout, CRUD, upload (Hệ thống ATTT)
-- ============================================================

CREATE TABLE activity_logs (
                                   id          UUID        PRIMARY KEY DEFAULT gen_random_uuid(),
                                   admin_id    UUID        CONSTRAINT fk_activity_logs_admin REFERENCES admins (id) ON DELETE SET NULL,
                                   action      log_action  NOT NULL,
                                   entity      VARCHAR(64),
                                   entity_id   UUID,
                                   detail      JSONB,
                                   ip_address  INET,
                                   created_at  TIMESTAMPTZ NOT NULL DEFAULT NOW()
);

CREATE INDEX idx_logs_admin   ON activity_logs (admin_id);
CREATE INDEX idx_logs_action  ON activity_logs (action);
CREATE INDEX idx_logs_created ON activity_logs (created_at DESC);
CREATE INDEX idx_logs_detail  ON activity_logs USING GIN (detail);

-- ============================================================
-- 13. AUTO-UPDATE updated_at TRIGGER
-- ============================================================

CREATE OR REPLACE FUNCTION set_updated_at()
RETURNS TRIGGER LANGUAGE plpgsql AS $$
BEGIN
    NEW.updated_at = NOW();
RETURN NEW;
END;
$$;

CREATE TRIGGER trg_admins_updated_at
    BEFORE UPDATE ON admins
    FOR EACH ROW EXECUTE FUNCTION set_updated_at();

CREATE TRIGGER trg_project_info_updated_at
    BEFORE UPDATE ON project_info
    FOR EACH ROW EXECUTE FUNCTION set_updated_at();

CREATE TRIGGER trg_site_settings_updated_at
    BEFORE UPDATE ON site_settings
    FOR EACH ROW EXECUTE FUNCTION set_updated_at();

CREATE TRIGGER trg_products_updated_at
    BEFORE UPDATE ON products
    FOR EACH ROW EXECUTE FUNCTION set_updated_at();

-- ============================================================
-- 14. SEED DATA
-- ============================================================

-- Note: The created_by / updated_by fields are left null for the initial seed data.
INSERT INTO admins (username, password_hash, full_name)
VALUES ('admin', '$2b$12$PlaceholderHashReplaceMe', 'Super Admin');

INSERT INTO project_info (name, developer, scale, description, map_lat, map_lng)
VALUES (
           'ERO Riverside Hà Nội – Bắc Ninh',
           'ERO Group',
           'Khu đô thị 50ha với 500 sản phẩm',
           'Dự án khu đô thị ERO Riverside toạ lạc tại vị trí kết nối Hà Nội – Bắc Ninh.',
           21.0245,
           105.8412
       );

INSERT INTO site_settings (hotline, email, address, zalo_link, facebook_link, tagline)
VALUES (
           '1900 xxxx',
           'contact@ero-riverside.vn',
           'Hà Nội – Bắc Ninh, Việt Nam',
           'https://zalo.me/ero-riverside',
           'https://facebook.com/ero-riverside',
           'Không gian sống lý tưởng ven sông'
       );

-- ============================================================
-- END OF SCHEMA
-- ============================================================