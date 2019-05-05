package com.hank.web;

import java.io.InputStream;

import org.apache.ibatis.session.SqlSession;
import org.apache.ibatis.session.SqlSessionFactory;
import org.apache.ibatis.session.SqlSessionFactoryBuilder;

public class HKSqlSessionFaction {
	static SqlSessionFactory sessionFactory = null;

	// ���巽��
	public static SqlSessionFactory getFactory() {
		String resource = "conf.xml";
		// ��ȡ�����ļ���Ϊ�˽����Ŀ�ڷ����Ժ��·���������������ǿ���ȥʹ�÷����������װ
		InputStream is = HKSqlSessionFaction.class.getClassLoader().getResourceAsStream(resource);
		SqlSessionFactory factory = new SqlSessionFactoryBuilder().build(is);
		return factory;
	}

	public static void init() {
		sessionFactory = getFactory();
	}

	public static SqlSession getSession() {
		return sessionFactory.openSession();
	}
}
