pipeline {
    agent any

    stages {
        stage('Testy PR') {
            steps {
                echo 'Uruchamianie testów dla Pull Requesta...'
                sh '''
                    mkdir -p build
                    cd build
                    cmake ..
                    cmake --build .
                    ctest --output-on-failure
                '''
                sh 'echo "Testy zakończone sukcesem!"'
            }
        }
    }
}
