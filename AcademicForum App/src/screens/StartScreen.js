import React, {useState, useRef} from "react";
import {I18nextProvider, useTranslation} from 'react-i18next';
import {Platform} from "react-native";
import End from "../components/End";
import MyModal from '../components/Modal';
import {FontAwesome} from '@expo/vector-icons';
import { StatusBar } from 'expo-status-bar';
import {
    StyleSheet,
    SafeAreaView,
    Image,
    ScrollView,
    View,
    Text,
    Dimensions,
    TouchableOpacity,
} from "react-native";
import Modal from 'react-native-modal';

const window = Dimensions.get('window');

export default function App({navigation}) {
    const {t, i18n} = useTranslation();
    const getTextAlignment = () => {
        const { i18n } = useTranslation();
      
        if (i18n.language === 'AR' || i18n.language === 'HE') {
          return 'right'; // Right-to-left alignment for Spanish
        } else {
          return 'left'; // Left-to-right alignment for other languages
        }
      };
    const textAlignment = getTextAlignment();
    const [modalVisible, setModalVisible] = useState(false);
    const [errorResponse, setErrorResponse] = useState(false);
    const [isSideBarOpen, setSideBarOpen] = useState(false);
    const [isConfirmed, setIsConfirmed] = useState(false);
    const [message, setMessage]  = useState('');
    const sidebarRef = useRef(null);
    const handleMenuPress = () => {
        setSideBarOpen(!isSideBarOpen);
    }
    const handleLanguageChange = (language) => {
        i18n.changeLanguage(language);
        setSideBarOpen(false);
    };
    const toggleModal = () => {
        setModalVisible(!modalVisible);
    }
    const data = [
        { id: 1, text: t('homepage.goals.0.desc') },
        { id: 2, text: t('homepage.goals.1.desc') },
        { id: 3, text: t('homepage.goals.2.desc') },
      ];
    const founders = t("homepage.founders" , {returnObjects: true});
    const cofounders = t("homepage.cofounders" , {returnObjects: true});
      // Render each item in the list
      const renderItem = ({ item }) => {
        const key = item.id.toString(); // Assign the key directly
      
        if (i18n.language === 'AR') {
          return (
            <View key={key} style={styles.listItem}>
              <Text style={[styles.text, { textAlign: textAlignment }]}>{item.text}</Text>
              <View style={{ width: 30 }} />
              <View style={styles.square}>
                <Text style={[styles.number]}>{item.id}</Text>
              </View>
            </View>
          );
        } else if (i18n.language === 'EN') {
          return (
            <View key={key} style={styles.listItem}>
              <View style={styles.square}>
                <Text style={[styles.number]}>{item.id}</Text>
              </View>
              <View style={{ width: 30 }} />
              <Text style={[styles.text, { textAlign: textAlignment }]}>{item.text}</Text>
            </View>
          );
        } else {
          // Default case if language is not Arabic, Hebrew, or English
          return (
            <View key={key} style={styles.listItem}>
              <Text style={[styles.text, { textAlign: textAlignment }]}>{item.text}</Text>
              <View style={{ width: 30 }} />
              <View style={styles.square}>
                <Text style={[styles.number]}>{item.id}</Text>
              </View>
            </View>
          );
        }
      };
    const renderFounders = (founders) => {
        return founders.map((founder, index) => (
            <View key={index} style={{ padding: 20 }}>
                <View style={styles.squ}>
                    <Image
                        source={{uri: founder.image}}
                        style={{ width: 300, height: 328, position: 'absolute', top: 0 }}
                    />
                    <View style={{ height: 350 }} />
                    <Text style={styles.Names}>{founder.name}</Text>
                    <View style={{ height: 30 }} />
                    <Text style={[styles.text, { textAlign: textAlignment }]}>
                        {founder.desc}
                    </Text>
                    <View style={{ height: 20 }} />
                </View>
            </View>
        ));
    };

    const renderCoFounders = (cofounders) => {
        return cofounders.map((cofounder, index) => (
            <View key={index} style={{ padding: 20 }}>
                <View style={styles.squ}>
                    <Image
                        source={{uri: cofounder.image}}
                        style={{ width: 300, height: 328, position: 'absolute', top: 0 }}
                    />
                    <View style={{ height: 350 }} />
                    <Text style={styles.Names}>{cofounder.name}</Text>
                    <View style={{ height: 30 }} />
                    <Text style={[styles.text, { textAlign: textAlignment }]}>
                        {cofounder.desc}
                    </Text>
                    <View style={{ height: 20 }} />
                </View>
            </View>
        ));
    };

    const scrollViewRef = useRef(null);
    return (
        <I18nextProvider i18n={i18n}>
            <SafeAreaView style={styles.container}>
                <StatusBar style="dark" />
                <View style={styles.sideBarContainer}>
                    <TouchableOpacity onPress={handleMenuPress} style={styles.menuButton}>
                        {isSideBarOpen ? (
                            <FontAwesome name="times" size={24} color="black"/>
                        ) : (
                            <FontAwesome name="bars" size={24} color="black"/>
                        )}
                    </TouchableOpacity>
                    {isSideBarOpen && (
                        <ScrollView
                            ref={sidebarRef}
                            contentContainerStyle={styles.sidebarContentContainer}
                        >
                            <View style={styles.languageContainer}>
                                <TouchableOpacity
                                    onPress={() => handleLanguageChange('EN')}
                                    style={styles.sidebarButton}
                                >
                                    <Text style={styles.sidebarButtonText}>EN</Text>
                                </TouchableOpacity>
                                <TouchableOpacity
                                    onPress={() => handleLanguageChange('AR')}
                                    style={styles.sidebarButton}
                                >
                                    <Text style={styles.sidebarButtonText}>AR</Text>
                                </TouchableOpacity>
                                <TouchableOpacity
                                    onPress={() => handleLanguageChange('HE')}
                                    style={styles.sidebarButton}
                                >
                                    <Text style={styles.sidebarButtonText}>HE</Text>
                                </TouchableOpacity>
                            </View>
                            <TouchableOpacity
                                onPress={() => {
                                    handleMenuPress();
                                    navigation.navigate('SearchScreen');
                                }}
                                style={styles.sidebarButton}
                            >
                                <Text style={styles.sidebarButtonText}>{t('navbar.academic')}</Text>
                            </TouchableOpacity>
                            <TouchableOpacity
                                onPress={() => {
                                    handleMenuPress();
                                    navigation.navigate('PodcastScreen');
                                }}
                                style={styles.sidebarButton}
                            >
                                <Text style={styles.sidebarButtonText}>{t('navbar.podcast')}</Text>
                            </TouchableOpacity>
                            <TouchableOpacity
                                onPress={() => {
                                    handleMenuPress();
                                    navigation.navigate('ContactUsScreen');
                                }}
                                style={styles.sidebarButton}
                            >
                                <Text style={styles.sidebarButtonText}>{t('navbar.contact')}</Text>
                            </TouchableOpacity>
                        </ScrollView>
                    )}
                </View>
                <ScrollView
                    nestedScrollEnabled
                    contentContainerStyle={styles.pageContainer}
                    showsVerticalScrollIndicator={false}
                >
                    <Image source={require("../../assets/FinalLogo.png")} style={styles.logo}/>
                    <View style={styles.sliderContainer}>
                        <ScrollView
                            ref={scrollViewRef}
                            nestedScrollEnabled
                            horizontal
                            pagingEnabled
                            showsHorizontalScrollIndicator={false}
                        >
                        </ScrollView>
                        <View style={{height: 20}}/>
                        <View style={styles.slideTextContainer}>
                            <Text style={styles.slideText}>{t('homepage.title')}</Text>
                            <Text style={styles.slideText}>{t('homepage.title_1')}</Text>
                            <Text style={styles.slideText}>{t('homepage.description')}</Text>
                        </View>
                        <View style={{height: 30}}/>
                        <TouchableOpacity onPress={toggleModal} style={styles.joinButton}>
                            <Text style={styles.joinButtonText}>{t('homepage.joinus')}</Text>
                        </TouchableOpacity>
                        <MyModal 
                            modalVisible={modalVisible} 
                            toggleModal={toggleModal} 
                            setIsConfirmed={setIsConfirmed} 
                            setMessage={setMessage}
                            setErrorResponse={setErrorResponse}
                        />
                    </View>
                    <View style={{height: 200}}/>
                    <Image source={require("../../assets/almuntda.png")} style={{width: 350, height: 250}}/>
                    <View style={styles.slideIntroductionContainer}>
                        <View style={{height: 10}}/>
                        <Text style={[styles.title ,{ textAlign: textAlignment }]}>{t('homepage.about')}</Text>
                        <View style={{height: 10}}/>
                        <Text style={[styles.introduction,,{ textAlign: textAlignment }]}>
                            {t('homepage.description')}
                        </Text>
                        <View style={{height: 50}}/>
                        <Text style={[styles.title,{ textAlign: textAlignment }]}>{t('homepage.about_2')}</Text>
                        <View style={{height: 20}}/>
                        <Text style={[styles.introduction,,{ textAlign: textAlignment }]}>
                            {t('homepage.description_2')}
                        </Text>
                    </View>
                    <View style={{marginTop: 20, paddingHorizontal: 20,}}>
                        <View style={{height: 70}}/>
                        <Image
                            source={require('../../assets/ourMessage.png')}
                            style={{width: 300, height: 300, marginLeft: 20}}
                        />
                        <View style={{height: 20}}/>
                        <View>
                            <Text style={[{
                                fontSize: 30,
                                color: "#f58723",
                                fontWeight: "bold",
                            },{ textAlign: textAlignment }]}>{t('homepage.vision.sub.title')}</Text>
                            <Text style={[styles.introduction,,{ textAlign: textAlignment }]}>
                                {t('homepage.vision.desc')}
                            </Text>
                            <View style={{height: 40}}/>
                            <Text style={[{
                                fontSize: 30,
                                color: "#f58723",
                                fontWeight: "bold",
                            },{ textAlign: textAlignment }]}>{t('homepage.message.sub.title')}</Text>
                            <Text style={[styles.introduction,{ textAlign: textAlignment }]}>
                                {t('homepage.message.desc')}
                            </Text>
                        </View>
                    </View>
                    <View style={{marginTop: 20, padding: 30,}}>
                        <View style={{height: 10}}/>
                        <Text style={[{
                            fontSize: 30,
                            color: "#f58723",
                            fontWeight: "bold",
                        },{ textAlign: textAlignment }]}>{t('homepage.goals.title')}</Text>
                        <View style={{flexDirection: 'column',}}>
                        {data.map(item => renderItem({ item}))}
                        </View>
                        <View style={{height: 30}}/>
                        <Image
                            source={require('../../assets/goals.png')}
                            style={{width: 350, height: 310,}}
                        />
                    </View>
                    <View style={{marginTop: 20, paddingHorizontal: 20,}}>
                        <View style={{height: 30}}/>
                        <Text style={[styles.title,{ textAlign: textAlignment }]}>{t('homepage.impact.sub.title')}</Text>
                        <View style={{height: 30}}/>
                        <Text style={[styles.introduction,{ textAlign: textAlignment }]}>
                            {t('homepage.impact.desc')} </Text>
                    </View>
                    <View style={{height: 30}}/>
                    <Image
                        source={require('../../assets/impact.png')}
                        style={{width: 300, height: 300, marginLeft: 20}}
                    />
                    <View style={{marginTop: 20, paddingHorizontal: 20,}}>
                        <View>
                            <Text style={[styles.title,{ textAlign: textAlignment }]}>{t('homepage.missfix.sub.title')}</Text>
                            <View style={{height: 30}}/>
                            <Text style={[styles.introduction,{ textAlign: textAlignment }]}>
                                {t('homepage.missfix.desc')} </Text>
                        </View>
                    </View>
                    <View style={{height: 30}}/>
                    <Image
                        source={require('../../assets/Alaa2.png')}
                        style={{width: 370, height: 350}}
                    />
                    <View style={{marginTop: 40, paddingHorizontal: 20}}>
                        <Text style={[styles.title, {fontSize: 35},{ textAlign: textAlignment }]}>{t('homepage.founder.title')}</Text>
                    </View>
                    {renderFounders(founders)}
                    <View style={{marginTop: 40, paddingHorizontal: 20}}>
                        <Text style={[styles.title, {fontSize: 35},{ textAlign: textAlignment }]}>{t('homepage.cofounders.title')}</Text>
                    </View>
                    {renderCoFounders(cofounders)}
                    <Modal isVisible={isConfirmed}>
                    <View style={styles.modalContainer}>
                      <Text style={styles.modalText}>{message}</Text>
                      <TouchableOpacity style={errorResponse? styles.modalButtonError : styles.modalButton} onPress={() => setIsConfirmed(false)}>
                        <Text style={styles.modalButtonText}>Close</Text>
                      </TouchableOpacity>
                    </View>
                    </Modal>
                    <End style={styles.end} navigation={navigation}/>
                </ScrollView>
            </SafeAreaView>
        </I18nextProvider>
    );
}
const styles = StyleSheet.create({
    container: {
        flex: 1,
        backgroundColor: "white",
        flexGrow: 1,
        ...Platform.select({
            android: {
                paddingTop: 40,
            },
        }),
    },
    languageContainer: {
        flexDirection: 'row',
        justifyContent: 'space-between',
    },
    sideBarContainer: {
        flexGrow: 1,
    },
    sidebarContentContainer: {
        flexGrow: 1,
        paddingLeft: 64,
        paddingRight: 64,
        width: '100%',
        height: '100%',
    },
    menuButton: {
        paddingLeft: 16,
        paddingTop: 8,
        height: 40
    },
    sidebarButton: {
        marginTop: 8,
        marginBottom: 16,
        paddingVertical: 8,
        paddingHorizontal: 16,
        backgroundColor: '#f58723',
        borderRadius: 8,
    },
    sidebarButtonText: {
        alignSelf: 'center',
        fontSize: 16,
        fontWeight: 'bold',
        color: 'white',
    },
    joinButton: {
        backgroundColor: "#041041",
        borderRadius: 8,
        paddingVertical: 12,
        paddingHorizontal: 16,
        top: 100,
        //height:60,
    },
    joinButtonText: {
        fontSize: 16,
        fontWeight: "bold",
        color: "white",
        padding: 10,
    },
    pageContainer: {
        flexGrow: 1,
        alignItems: "center",
        paddingBottom: 20,
        zIndex: 0,
    },
    logo: {
        width: 200,
        height: 75,
        resizeMode: "contain",
        marginTop: 20,
    },
    sliderContainer: {
        height: Dimensions.get("window").height / 3,
        //width: "100%",
        marginTop: 10,
        alignItems: 'center',
    },
    slide: {
        flex: 1,
        alignItems: "center",
        justifyContent: "center",
        width: Dimensions.get("window").width,
    },
    image: {
        width: "100%",
        height: "100%",
        resizeMode: "cover",
    },
    slideTextContainer: {
        position: "absolute",
        top: 50,
        width: "100%",
        alignItems: "center",
    },
    slideText: {
        paddingVertical: 10,
        fontSize: 25,
        fontWeight: "bold",
        color: '#041041',
        padding: 10,
        borderColor: "blue",
        textAlign: "center",
    },
    slideIntroductionContainer: {
        marginTop: 20,
        paddingHorizontal: 20,
        top: 60,
        //textAlign: "left",
    },
    title: {
        fontSize: 25,
        fontWeight: "bold",
        color: "#f58723",
        //top: 40,
        marginBottom: 10,
        // textAlign: "left",
    },
    introduction: {
        //top: 20,
        fontSize: 20,
        fontWeight: "bold",
        color: "black",
        //textAlign: "left",
        padding: 10,
    },
    listItem: {
        top: 15,
        flexDirection: 'row',
        alignItems: 'center',
        marginBottom: 10,
        height: 320,
        width: 300,
    },
    number: {
        fontWeight: 'bold',
        left: 5,
        fontSize: 16,
        marginRight: 10,
        color: "#f58723",
        textAlign: "center",
    },
    rectangular: {
        width: Dimensions.get("window").width,
        backgroundColor: '#F7FAF8',
        flexDirection: 'row',
        alignSelf: 'flex-start',
    },
    text: {
        fontSize: 20,
        left: 10,
        top: 10,
        
        fontWeight: "bold",
        padding: 10,
    },
    square: {
        width: 20,
        height: 30,
        backgroundColor: 'lightgray',
        justifyContent: 'center',
        alignItems: 'center',
        borderRadius: 10,
    },
    squ: {
        marginBottom: 10,
        backgroundColor: '#fff',
        borderRadius: 8,
        shadowOpacity: 0.7,
        shadowRadius: 10,
        padding: 10,
        shadowColor: '#000',
        shadowOffset: { width: 0, height: 2 },
        elevation: 2,
         width: 300,
        alignSelf: 'flex-start',
    
    },
    squar: {
        width: 300,
        alignSelf: 'flex-start',
        shadowColor: '#000000',
        shadowOpacity: 0.7,
        shadowRadius: 10,
        elevation: 0.5, // for Android shadows
        padding: 5,
        justifyContent: 'center',
        alignItems: 'center',
        borderColor: "black",
        top: 50,
        bottom: 50,
    },
    Names: {
        top: 20,
        textAlign: "center",
        fontWeight: "bold",
        fontSize: 20,
    },
    halfReg: {
        flexDirection: 'column',
        width: (Dimensions.get("window").width / 2) + 10,
        padding: 20,
    },
    TextInfo: {
        fontSize: 18,
        color: "#05063F",
        textAlign: 'center',
        fontWeight: "bold",
    },
    info: {
        flexDirection: 'row',
    },
    nfo: {
        fontSize: 14,
        fontWeight: "bold",
    },
    end: {
        width: Dimensions.get("window").width,
        backgroundColor: '#092D82',
        flexDirection: 'row',
        alignSelf: 'flex-start',
        bottom: 0,
        position: 'absolute',
    },
    TextEnd: {
        color: 'white',
    },
    modalContainer: {
        backgroundColor: 'white',
        padding: 16,
        alignItems: 'center',
        justifyContent: 'center',
        borderRadius: 8,
      },
      modalText: {
        fontSize: 20,
        fontWeight: 'bold',
        marginBottom: 16,
        alignSelf: 'center',
      },
      modalButton: {
        backgroundColor: 'green',
        paddingVertical: 8,
        paddingHorizontal: 16,
        borderRadius: 8,
      },
      modalButtonError: {
        backgroundColor: 'red',
        paddingVertical: 8,
        paddingHorizontal: 16,
        borderRadius: 8,
      },
      modalButtonText: {
        color: 'white',
        fontSize: 16,
        fontWeight: 'bold',
      },
});