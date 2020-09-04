CREATE INDEX ind_pub_type ON pub USING hash (pub_type);

CREATE INDEX ind_pub_key ON pub(pub_key);

CREATE INDEX ind_field_name ON field(field_name);

CREATE INDEX ind_field_pub_key ON field(pub_key);
