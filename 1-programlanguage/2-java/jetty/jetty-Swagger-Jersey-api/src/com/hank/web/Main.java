package com.hank.web;

import org.eclipse.jetty.server.Connector;
import org.eclipse.jetty.server.Server;
import org.eclipse.jetty.server.nio.SelectChannelConnector;
import org.eclipse.jetty.webapp.WebAppContext;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class Main {
	static Logger loggger = LoggerFactory.getLogger(Main.class);

	public static void main(String[] args) {
		// String resource =
		// "C:\\Users\\cj\\workspace-jee\\jetty-webSmartHomeService\\src\\conf.xml";
		// InputStream is = null;
		// try {
		// is = new FileInputStream(new File(resource));
		// } catch (FileNotFoundException e1) {
		// e1.printStackTrace();
		// return;
		// }
		//
		// // ����sqlSession�Ĺ���
		// SqlSessionFactory sessionFactory = new
		// SqlSessionFactoryBuilder().build(is);
		// // ʹ��MyBatis�ṩ��Resources�����mybatis�������ļ�����Ҳ���ع�����ӳ���ļ���
		// // Reader reader = Resources.getResourceAsReader(resource);
		// // ����sqlSession�Ĺ���
		// // SqlSessionFactory sessionFactory = new
		// // SqlSessionFactoryBuilder().build(reader);
		// // ������ִ��ӳ���ļ���sql��sqlSession
		// SqlSession session = sessionFactory.openSession();
		// /**
		// * ӳ��sql�ı�ʶ�ַ�����
		// * me.gacl.mapping.userMapper��userMapper.xml�ļ���mapper��ǩ��namespace���Ե�ֵ��
		// * getUser��select��ǩ��id����ֵ��ͨ��select��ǩ��id����ֵ�Ϳ����ҵ�Ҫִ�е�SQL
		// */
		// String statement = "com.hank.entity.gatewayMapper.getGateway";//
		// ӳ��sql�ı�ʶ�ַ���
		// // ִ�в�ѯ����һ��Ψһuser�����sql
		// com.hank.entity.GatewayInfo gateway = session.selectOne(statement,
		// "HK0120162000001");
		// loggger.debug(gateway.toString());
		// session.close();

		///////////////////////////////////////////////////////////
		Server server = new Server();
		Connector connector = new SelectChannelConnector();
		connector.setPort(8080);
		server.addConnector(connector);

		WebAppContext webapp = new WebAppContext();
		webapp.setContextPath("/");
		webapp.setResourceBase("/opt/project-jee/jetty-webSmartHomeService/WebContent");
		webapp.setDescriptor("/opt/project-jee/jetty-webSmartHomeService/WebContent/web.xml");
		webapp.setParentLoaderPriority(true);

//		EnumSet<DispatcherType> set = EnumSet.allOf(DispatcherType.class);
//
//		webapp.addFilter(new FilterHolder(new Filter() {
//
//			@Override
//			public void init(FilterConfig arg0) throws ServletException {
//
//			}
//
//			@Override
//			public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain)
//					throws IOException, ServletException {
//				request.setAttribute("APOLLO_BROKER", "");
//				System.out.println("->................" + request.getParameterMap().toString());
//				chain.doFilter(request, response);
//			}
//
//			@Override
//			public void destroy() {
//
//			}
//		}), "/*", set);

		server.setHandler(webapp);
		try {
			server.start();
			server.join();
		} catch (InterruptedException e) {
			e.printStackTrace();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
